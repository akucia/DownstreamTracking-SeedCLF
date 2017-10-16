"""
My version of hep_ml.speedup module
link to source: https://github.com/arogozhnikov/hep_ml/blob/master/hep_ml/speedup.py
author of original code: Alex Rogozhnikov
##########################################################################################
**hep_ml.speedup** is module to obtain formulas with machine learning,
which can be applied very fast (with a speed comparable to simple selections),
while keeping high quality of classification.

In many application (i.e. triggers in HEP) it is pressing to get really fast formula.
This module contains tools to prepare formulas, which can be applied with the speed comparable to cuts.

Example
-------
Let's show how one can use some really heavy classifier and still have fast predictions:

>>> from sklearn.ensemble import RandomForestClassifier
>>> from hep_ml.speedup import LookupClassifier
>>> base_classifier = RandomForestClassifier(n_estimators=1000, max_depth=25)
>>> classifier = LookupClassifier(base_estimator=base_classifier, keep_trained_estimator=False)
>>> classifier.fit(X, y, sample_weight=sample_weight)

Though training takes much time, all predictions are precomputed and saved to lookup table,
so you are able to predict millions of events per second using single CPU:

>>> classifier.predict_proba(testX)

"""

from __future__ import division, print_function, absolute_import
import numpy
import pandas
from collections import OrderedDict
from sklearn.base import ClassifierMixin, BaseEstimator, clone

__author__ = 'Alex Rogozhnikov'

def to_pandas_dataframe(X):
    """
    Convert 2-dimensional array to DataFrame. If input was a DataFrame, returns itself.
    """
    if isinstance(X, pandas.DataFrame):
        return X
    else:
        return pandas.DataFrame(X, columns=['Feature{}'.format(i) for i in range(X.shape[1])])

def check_xyw(X, y, sample_weight=None, classification=False, allow_multiple_outputs=False):
    """Checks parameters of classifier / loss / metrics.
    :param X: array-like of shape [n_samples, n_features] (numpy.array or pandas.DataFrame)
    :param y: array-like of shape [n_samples]
    :param sample_weight: None or array-like of shape [n_samples]
    :return: normalized 3-tuple (X, y, sample_weight)
    """

    y = numpy.array(y)
    if not allow_multiple_outputs:
        assert numpy.ndim(y) == 1, 'y should be one-dimensional'
    sample_weight = check_sample_weight(y, sample_weight=sample_weight)

    # only pandas.DataFrame and numpy.array are allowed. No checks on sparsity here.
    if not (isinstance(X, pandas.DataFrame) or isinstance(X, numpy.ndarray)):
        X = numpy.array(X)
    if classification:
        y = numpy.array(y, dtype=int)

    assert len(X) == len(y), 'lengths are different: {} and {}'.format(len(X), len(y))
    assert numpy.ndim(X) == 2, 'X should have 2 dimensions'

    return X, y, sample_weight

def check_sample_weight(y_true, sample_weight, normalize=False, normalize_by_class=False):
    """Checks the weights, returns normalized version
    :param y_true: numpy.array of shape [n_samples]
    :param sample_weight: array-like of shape [n_samples] or None
    :param normalize: bool, if True, will scale everything to mean = 1.
    :param normalize_by_class: bool, if set, will set equal weight = 1 for each value of y_true.
        Better to use normalize if normalize_by_class is used.
    :returns: numpy.array with weights of shape [n_samples]"""
    if sample_weight is None:
        sample_weight = numpy.ones(len(y_true), dtype=numpy.float)
    else:
        sample_weight = numpy.array(sample_weight, dtype=numpy.float)
        assert numpy.ndim(sample_weight) == 1, 'weights vector should be 1-dimensional'
        assert len(y_true) == len(sample_weight), \
            "The length of weights is different: not {0}, but {1}".format(len(y_true), len(sample_weight))

    if normalize_by_class:
        sample_weight = numpy.copy(sample_weight)
        for value in numpy.unique(y_true):
            sample_weight[y_true == value] /= numpy.sum(sample_weight[y_true == value])

    if normalize:
        sample_weight = sample_weight / numpy.mean(sample_weight)

    return sample_weight

def weighted_quantile(array, quantiles, sample_weight=None, array_sorted=False, old_style=False):
    """ Very close to numpy.percentile, but supports weights.
    NOTE: quantiles should be in [0, 1]!
    :param array: numpy.array with data
    :param quantiles: array-like with many percentiles
    :param sample_weight: array-like of the same length as `array`
    :param array_sorted: bool, if True, then will avoid sorting
    :param old_style: if True, will correct output to be consistent with numpy.percentile.
    :return: numpy.array with computed percentiles.
    """
    array = numpy.array(array)
    quantiles = numpy.array(quantiles)
    sample_weight = check_sample_weight(array, sample_weight)
    assert numpy.all(quantiles >= 0) and numpy.all(quantiles <= 1), 'Percentiles should be in [0, 1]'

    if not array_sorted:
        sorter = numpy.argsort(array)
        array, sample_weight = array[sorter], sample_weight[sorter]

    weighted_quantiles = numpy.cumsum(sample_weight) - 0.5 * sample_weight
    if old_style:
        # To be convenient with numpy.percentile
        weighted_quantiles -= weighted_quantiles[0]
        weighted_quantiles /= weighted_quantiles[-1]
    else:
        weighted_quantiles /= numpy.sum(sample_weight)
    return numpy.interp(quantiles, weighted_quantiles, array)


class LookupClassifier(BaseEstimator, ClassifierMixin):
    def __init__(self, base_estimator, n_bins=16, max_cells=500000000, keep_trained_estimator=True):
        """
        LookupClassifier splits each of features into bins, trains a base_estimator to use this data.
        To predict class for new observation, results of base_estimator are kept for all possible combinations of bins,
        and saved together

        :param base_estimator: classifier used to build predictions
        :param n_bins:

            * int: how many bins to use for each axis
            * dict: feature_name -> int, specialize how many bins to use for each axis
            * dict: feature_name -> list of floats, set manually edges of bins

            By default, the (weighted) quantiles are used to compute bin edges.
        :type n_bins: int | dict

        :param int max_cells: raise error if lookup table will have more items.
        :param bool keep_trained_estimator: if True, trained estimator will be saved.

        See also: this idea is used inside LHCb triggers, see V. Gligorov, M. Williams, 'Bonsai BDT'

        Resulting formula is very simple and can be rewritten in other language or environment (C++, CUDA, etc).

        """
        self.base_estimator = base_estimator
        self.n_bins = n_bins
        self.max_cells = max_cells
        self.keep_trained_estimator = keep_trained_estimator

    def check_dimensions(self, bin_edges):
        cumulative_size = numpy.cumprod([len(bin_edge) + 1 for name, bin_edge in bin_edges.items()])
        if numpy.any(cumulative_size > self.max_cells):
            raise ValueError('the total size of lookup table exceeds {}, '
                             'reduce n_bins or number of features in use'.format(self.max_cells))

    def fit(self, X, y, sample_weight=None):
        """Train a classifier and collect predictions for all possible combinations.

        :param X: pandas.DataFrame or numpy.array with data of shape [n_samples, n_features]
        :param y: array with labels of shape [n_samples]
        :param sample_weight: None or array of shape [n_samples] with weights of events
        :return: self
        """
        self.classes_ = numpy.unique(y)
        X, y, normed_weights = check_xyw(X, y, sample_weight=sample_weight, classification=True)
        X = to_pandas_dataframe(X)
        normed_weights = check_sample_weight(y, sample_weight=normed_weights, normalize_by_class=True, normalize=True)

        self.bin_edges = self._compute_bin_edges(X, normed_weights=normed_weights)
        self.check_dimensions(self.bin_edges)

        n_parameter_combinations = numpy.prod([len(bin_edge) + 1 for name, bin_edge in self.bin_edges.items()])

        transformed_data = self.transform(X)
        trained_estimator = clone(self.base_estimator)
        fit_params = {}
        if sample_weight is not None:
            fit_params['sample_weights'] = sample_weight
        # trained_estimator.fit(transformed_data, y, **fit_params)

        all_lookup_indices = numpy.arange(int(n_parameter_combinations))
        all_combinations = self.convert_lookup_index_to_bins(all_lookup_indices)
        self._lookup_table = trained_estimator.predict_proba(all_combinations)

        if self.keep_trained_estimator:
            self.trained_estimator = trained_estimator

        return self

    def _compute_bin_edges(self, X, normed_weights):
        """
        Compute edges of bins, weighted quantiles are used,
        """
        bins_over_axis = OrderedDict()
        for column in X.columns:
            if isinstance(self.n_bins, int):
                bins_over_axis[column] = self.n_bins
            else:
                bins_over_axis[column] = self.n_bins[column]

        bin_edges = OrderedDict()
        for column, column_bins in bins_over_axis.items():
            if isinstance(column_bins, int):
                quantiles = numpy.linspace(0., 1., column_bins + 1)[1:-1]
                bin_edges[column] = weighted_quantile(X[column], quantiles=quantiles, sample_weight=normed_weights)
            else:
                bin_edges[column] = numpy.array(list(column_bins))

        return bin_edges

    def convert_bins_to_lookup_index(self, bins_indices):
        """
        :param bins_indices: numpy.array of shape [n_samples, n_columns], filled with indices of bins.
        :return: numpy.array of shape [n_samples] with corresponding index in lookup table
        """
        lookup_indices = numpy.zeros(len(bins_indices), dtype=int)
        bins_indices = numpy.array(bins_indices)
        assert bins_indices.shape[1] == len(self.bin_edges)
        for i, (column_name, bin_edges) in enumerate(self.bin_edges.items()):
            lookup_indices *= len(bin_edges) + 1
            lookup_indices += bins_indices[:, i]
        return lookup_indices

    def convert_lookup_index_to_bins(self, lookup_indices):
        """
        :param lookup_indices: array of shape [n_samples] with positions at lookup table
        :return: array of shape [n_samples, n_features] with indices of bins.
        """

        result = numpy.zeros([len(lookup_indices), len(self.bin_edges)], dtype='uint8')
        for i, (column_name, bin_edges) in list(enumerate(self.bin_edges.items()))[::-1]:
            n_columns = len(bin_edges) + 1
            result[:, i] = lookup_indices % n_columns
            lookup_indices = lookup_indices // n_columns

        return result

    def transform(self, X):
        """Convert data to bin indices.

        :param X: pandas.DataFrame or numpy.array with data
        :return: numpy.array, where each column is replaced with index of bin
        """
        X = to_pandas_dataframe(X)
        assert list(X.columns) == list(self.bin_edges.keys()), 'passed dataset with wrong columns'
        result = numpy.zeros(X.shape, dtype='uint8')
        for i, column in enumerate(X.columns):
            edges = self.bin_edges[column]
            result[:, i] = numpy.searchsorted(edges, X[column])

        return result

    def predict(self, X):
        """Predict class for each event

        :param X: pandas.DataFrame with data
        :return: array of shape [n_samples] with predicted class labels.
        """
        return self.classes_[numpy.argmax(self.predict_proba(X), axis=1)]

    def predict_proba(self, X):
        """ Predict probabilities for new observations

        :param X: pandas.DataFrame with data
        :return: probabilities, array of shape [n_samples, n_classes]
        """
        bins_indices = self.transform(X)
        lookup_indices = self.convert_bins_to_lookup_index(bins_indices)
        return self._lookup_table[lookup_indices]