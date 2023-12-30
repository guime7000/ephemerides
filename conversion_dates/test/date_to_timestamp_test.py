from datetime import datetime
import os
from date_to_timestamp import convert_date_to_timestamp


def test_convert_date_to_timestamp():
    human_test_filepath = "./test/test_export_human_dates.csv"
    timestamp_test_filepath = "./test/timestamp_test_export.txt"
    convert_date_to_timestamp(human_test_filepath, timestamp_test_filepath)

    with open(timestamp_test_filepath, 'r', encoding='utf-8') as readfile:
        ts = readfile.readlines()

    assert ts[1].split(",")[0] == str(
        int(datetime(2023, 12, 25, 8, 0, 0).timestamp()))
