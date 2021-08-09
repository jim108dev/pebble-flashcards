#!/usr/bin/env python

"""
 Read feedback from a csv file. Integrate it in the current findings. Delete the file.
"""

import datetime
import logging
import os

import pandas as pd

from util import get_conf

logging.basicConfig(level=logging.DEBUG)

HISTORY_COLUMNS = ["id", "feedback_history", "start_history", "stop_history"]

NOW = (datetime.datetime.now() - datetime.datetime(1970, 1, 1)).total_seconds()


def main(conf):
    ref_df = pd.read_csv(conf.reference_filename, sep=';')

    history_df = pd.DataFrame(columns=HISTORY_COLUMNS) if not os.path.isfile(
        conf.history_filename) else pd.read_csv(conf.history_filename, sep=';')

    df = ref_df.merge(history_df, on='id', how="left")

    logging.debug(df.head())

    df.to_csv(path_or_buf=conf.history_filename,
              columns=HISTORY_COLUMNS, index=False, sep=";")


if __name__ == "__main__":
    main(get_conf())
