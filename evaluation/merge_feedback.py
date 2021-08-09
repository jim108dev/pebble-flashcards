#!/usr/bin/env python

"""
 Read feedback from a csv file. Integrate it in the current findings. Delete the file.
"""

import logging
import os
import os.path
import shutil
import tempfile

import pandas as pd
from pandas.core.dtypes.missing import isna

from util import get_conf

logging.basicConfig(level=logging.DEBUG)


OUTPUT_COLUMNS = ["id", "feedback_history", "start_history", "stop_history", "buried"]

# https://note.nkmk.me/en/python-check-int-float/
def is_integer(n):
    try:
        float(n)
    except ValueError:
        return False
    else:
        return float(n).is_integer()


def to_string(x):
    return str(int(x)) if is_integer(x) else x


def append(xs, x):

    if isna(x):
        return to_string(xs)

    if isna(xs):
        return to_string(x)

    return to_string(xs) + "|" + to_string(x)


def main(conf):
    if not os.path.isfile(conf.feedback_filename):
        logging.warn("%s does not exist" % conf.feedback_filename)
        return

    feedback_df = pd.read_csv(conf.feedback_filename, sep=";")
    history_df = pd.read_csv(conf.history_filename, sep=";")

    feedback_df["start"] = feedback_df["start"].astype(int).astype(str)
    feedback_df["stop"] = feedback_df["stop"].astype(int).astype(str)
    feedback_df["buried"] = feedback_df["feedback"] == 4

    df = history_df.merge(feedback_df, on="id", how="left")

    df["feedback_history"] = df.apply(
        lambda x: append(x["feedback_history"], x["feedback"]), axis=1
    )
    df["start_history"] = df.apply(
        lambda x: append(x["start_history"], x["start"]), axis=1
    )
    df["stop_history"] = df.apply(
        lambda x: append(x["stop_history"], x["stop"]), axis=1
    )
    df["buried"] = df["buried_x"] | df["buried_y"]

    logging.debug(df.head())
    df.to_csv(
        path_or_buf=conf.history_filename, columns=OUTPUT_COLUMNS, index=False, sep=";"
    )

    shutil.move(conf.feedback_filename, tempfile.gettempdir())


if __name__ == "__main__":
    main(get_conf())
