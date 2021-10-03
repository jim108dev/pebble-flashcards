#!/usr/bin/env python

"""
 Create a new file for the next session based on the history.
"""

import logging
from datetime import date

import numpy as np
import pandas as pd
from pandas.core.dtypes.missing import isna
from pandas.core.frame import DataFrame

from algo_sm2 import supermemo_2
from util import get_conf

logging.basicConfig(level=logging.DEBUG)

OUTPUT_COLUMNS = ["id", "text1", "text2", "feedback","start", "stop"]

NOW = date.today()

def countdown(last_stop, waiting_period):
    dt_last_stop = date.fromtimestamp(int(last_stop))
    delta = NOW - dt_last_stop
    days_past = delta.days
    return waiting_period - days_past

def check_len(df:DataFrame):
    too_long_mask = (df['id'].astype(str).str.len() > 15) | (df['text1'].astype(str).str.len() > 84) | (df['text2'].astype(str).str.len() > 84)

    too_long_df = df.loc[too_long_mask]
    if not too_long_df.empty:
        logging.warning("The following records are too long, the cannot be fully displayed:")
        logging.warning(too_long_df)

def main(conf):
    df = pd.read_csv(conf.history_filename, sep=';')

    df['waiting_period'] = df['feedback_history'].apply(
        lambda cs: 0 if isna(cs) else supermemo_2([int(float(v)) for v in str(cs).split('|')]))

    df['last_stop'] = df.apply(lambda x: pd.NA if isna(x['stop_history']) else int(float(str(x['stop_history']).split('|')[-1])), axis=1)
    df['last_start'] = df.apply(lambda x: pd.NA if isna(x['start_history']) else int(float(str(x['start_history']).split('|')[-1])), axis=1)

    df['countdown'] = df.apply(lambda x: 0 if isna(x['stop_history']) else countdown(x['last_stop'], x['waiting_period']), axis=1)

    df = df[df['buried'] == False]

    df = df[df['countdown'] <= 0]

    df['duration'] = df.apply(lambda x: 0 if isna(x['last_start']) or isna(x['last_stop']) else x['last_stop'] - x['last_start'], axis=1)

    #df = df.sort_values(by=['duration'], ascending=False)

    df = df.nsmallest(conf.max_records, 'countdown')

    ref_df = pd.read_csv(conf.reference_filename, sep=';')

    check_len(ref_df)

    df = df.merge(ref_df, on='id')

    df['feedback'] = 0
    df['start'] = 0
    df['stop'] = 0

    #https://stackoverflow.com/questions/29576430/shuffle-dataframe-rows
    df = df.sample(frac=1, replace=False).reset_index(drop=True)

    logging.debug(df.head())

    df.to_csv(path_or_buf=conf.next_session_filename, columns=OUTPUT_COLUMNS, index=False, sep=";")

if __name__ == "__main__":
    main(get_conf())
