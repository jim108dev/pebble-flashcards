#!/usr/bin/env python

"""
 Read feedback from a csv file. Integrate it in the current findings. Delete the file.
"""

import os
import os.path

import pandas as pd
from pandas.core.dtypes.missing import isna

from util import get_conf

OUTPUT_COLUMNS = ["id", "feedback_history", "start_history", "stop_history"]

def append(xs, x):

    if isna(x):
        return xs

    if isna(xs):
        return x

    return str(xs) + '|' + str(x)

def main(conf):
    if not os.path.isfile(conf.feedback_filename):
        print("%s does not exist" % conf.feedback_filename)
        return

    feedback_df = pd.read_csv(conf.feedback_filename, sep=';')
    history_df = pd.read_csv(conf.history_filename, sep=';')

    df = history_df.merge(feedback_df, on='id', how="left")

    df["feedback_history"] = df.apply(lambda x: append(x["feedback_history"],x["feedback"]), axis=1)
    df["start_history"] = df.apply(lambda x: append(x["start_history"],x["start"]), axis=1)
    df["stop_history"] = df.apply(lambda x: append(x["stop_history"],x["stop"]), axis=1)

    print(df.head())
    df.to_csv(path_or_buf=conf.history_filename, columns=OUTPUT_COLUMNS, index=False, sep=";")

    os.remove(conf.feedback_filename) 

if __name__ == "__main__":
    main(get_conf())

