import unittest

import pandas as pd

from data_pipeline import data_pipeline_all_labels


class TestPipeline(unittest.TestCase):
    def setUp(self):
        self.df = pd.DataFrame(
            index=[0, 1, 2, 3, 4],
            data={
                'seed_chi2PerDoF': [
                    1.224318, 2.310915, 6.570617, 0.769758, 1.118085
                ],
                'seed_p'         : [
                    24853.207031, 14115.601562, 140397.859375, 2739.445557, 2894.345459
                ],
                'seed_pt'        : [
                    4751.315918, 1086.456421, 32186.728516, 1166.308960, 1438.781494
                ],
                'seed_x'         : [
                    1218.260376, 174.162796, 1750.810547, -1143.220337, -1823.896362
                ],
                'seed_y'         : [
                    57.654774, 14.526168, 366.384735, 80.898964, -236.162720
                ],
                'seed_tx'        : [
                    0.194639, 0.077169, 0.219562, -0.470408, -0.571847
                ],
                'seed_ty'        : [
                    0.007061, 0.002084, 0.085235, 0.010258, -0.034705
                ],
            }
        )

    def test_pipeline(self):
        df_expected = pd.DataFrame(
            {
                "seed_chi2PerDoF": {
                    "0": -0.5247728821, "1": 0.1569133052,
                    "2": 1.8758790545, "3": -0.9023472552,
                    "4": -0.6056722223
                }, "seed_p"      : {
                "0": 0.4346671702, "1": 0.0492139026, "2": 1.6144490076,
                "3": -1.0679037954, "4": -1.030426285
            }, "seed_pt"         : {
                "0": 0.3354868762, "1": -0.8095394218, "2": 1.8201272209,
                "3": -0.754501638, "4": -0.5915730373
            }, "seed_tx"         : {
                "0": 0.8952448508, "1": 0.5502353611, "2": 0.9684437229,
                "3": -1.0579987528, "4": -1.355925182
            }, "seed_ty"         : {
                "0": -0.177047807, "1": -0.304281106, "2": 1.8214122927,
                "3": -0.0953188826, "4": -1.2447644971
            }, "seed_x"          : {
                "0": 0.8718064965, "1": 0.1023875563, "2": 1.2642546699,
                "3": -0.8684216415, "4": -1.3700270812
            }, "seed_y"          : {
                "0": 0.0051830753, "1": -0.2196169094, "2": 1.6143816311,
                "3": 0.1263391719, "4": -1.5262869689
            }, "seed_angle"      : {
                "0": -0.3453938219, "1": 0.0461078275, "2": 1.3874362054,
                "3": -1.6307126971, "4": 0.5425624861
            }, "seed_pr"         : {
                "0": -0.6202259452, "1": -1.2933413924, "2": -0.3898816273,
                "3": 0.8893873238, "4": 1.414061641
            }, "seed_r"          : {
                "0": -0.0233782751, "1": -1.762963381, "2": 0.9241380096,
                "3": -0.145823193, "4": 1.0080268395
            }
            }
        )
        pipeline = data_pipeline_all_labels()
        df_transformed = pipeline.fit_transform(self.df)
        for column in df_transformed.columns:
            for value_a, value_b in zip(df_transformed[column], df_expected[column]):
                self.assertAlmostEqual(
                    value_a, value_b,
                    msg="Error in feature: {}".format(column)
                )
        df_transformed = pipeline.transform(self.df)
        for column in df_transformed.columns:
            for value_a, value_b in zip(df_transformed[column], df_expected[column]):
                self.assertAlmostEqual(
                    value_a, value_b,
                    msg="Error in feature: {}".format(column)
                )


if __name__ == '__main__':
    unittest.main.runTests()
