#!/usr/bin/env python

"""
"""

import argparse
import logging
from configparser import ConfigParser


def get_conf():
    parser = argparse.ArgumentParser()
    parser.add_argument("config", help="Set the configuration file")
    conf = parser.parse_args()

    parser = ConfigParser()
    parser.read(conf.config)


    conf.reference_filename = parser.get('reference', 'filename')
    conf.history_filename = parser.get('history', 'filename')
    conf.feedback_filename = parser.get('feedback', 'filename')
    conf.next_session_filename = parser.get('next_session', 'filename')
    conf.max_records = parser.getint('next_session', 'max_records')

    logging.basicConfig(level=parser.get('main', 'logging'))
    return conf
