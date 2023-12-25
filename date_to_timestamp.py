import csv
import os
import datetime

# Path to where the ephemerides input file is
import_csv_filepath = "/home/tom/Projets_Artistiques/Birot/ephemerides/import/"
# Name of the CSV file containing the ephemerides
date_filename = "matrice_export_dates.csv"

dates_filepath = os.path.join(import_csv_filepath, date_filename)

# Path to where the timestamp array file will be saved
export_timestamp_path = "/home/tom/Projets_Artistiques/Birot/ephemerides/export/"
# Name of the TXT file containing the timestamp array
timestamp_filename = "timestamp_dates.txt"

timestamp_filepath = os.path.join(export_timestamp_path, timestamp_filename)

timestamps_export = []

with open(dates_filepath, encoding='UTF-8') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        year, month, day, hour, minutes, seconds = int(row['AAAA']), int(
            row['MM']), int(row['JJ']), int(row['h']), int(row['min']), int(row['secs'])
        import_date = datetime.datetime(
            year, month, day, hour, minutes, seconds)
        timestamps_export.append(str(int(import_date.timestamp())))

with open(timestamp_filepath, 'w', encoding='UTF-8') as export_file:
    for ts in timestamps_export:
        export_file.write(ts + "\n")
