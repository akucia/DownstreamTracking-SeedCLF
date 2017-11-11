import ROOT
import root_numpy
import sys
import pandas as pd

if (len(sys.argv)>1):
    file_path = sys.argv[1]
    print("Reading from file: {}".format(file_path))
    
    root_file = ROOT.TFile(file_path)
    root_file.cd("ToolSvc.PatDebugTTTruthTool") 
    tree = ROOT.gDirectory.Get("DownstreamSeedDebugTuple")
    data = pd.DataFrame(root_numpy.tree2array(tree))
    print(data.head(3))
    pkl_file_path = file_path.replace(".root",".pkl")
    data.to_pickle(pkl_file_path)
    print("file saved!")
    
else:
    print("No imput file provided!")
