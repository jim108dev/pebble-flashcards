#!/usr/bin/env python

"""
 Create a new file for the next session based on the history.
"""

from datetime import date

import pandas as pd
from pandas.core.dtypes.missing import isna

from algo_sm2 import supermemo_2
from util import get_conf

OUTPUT_COLUMNS = ["id", "text1", "text2", "feedback","start", "stop"]

NOW = date.today()

def countdown(last_stop, waiting_period):
    dt_last_stop = date.fromtimestamp(int(last_stop))
    delta = NOW - dt_last_stop
    days_past = delta.days
    return waiting_period - days_past


def main(conf):
    df = pd.read_csv(conf.history_filename, sep=';')

    df['waiting_period'] = df['feedback_history'].apply(
        lambda cs: 0 if isna(cs) else supermemo_2([int(v) for v in str(cs).split('|')]))

    df['countdown'] = df.apply(lambda x: 0 if isna(x['stop_history']) else countdown(
        x['stop_history'].split('|')[-1], x['waiting_period']), axis=1)

    df = df[df['countdown'] <= 1]
    df = df.nsmallest(conf.max_records, 'countdown')

    ref_df = pd.read_csv(conf.reference_filename, sep=';')

    df = df.merge(ref_df, on='id')

    df['feedback'] = 0
    df['start'] = 0
    df['stop'] = 0

    print(df.head())

    df.to_csv(path_or_buf=conf.next_session_filename, columns=OUTPUT_COLUMNS, index=False, sep=";")

if __name__ == "__main__":
    main(get_conf())
