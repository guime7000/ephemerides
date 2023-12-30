import csv
import os
import datetime

# ======== Eléments à modifier par l'utilisateur (si nécessaire) ===================

# 1. Chemin vers le répertoire contenant les fichiers des tops DEPART et STOP en temps "humain"
import_csv_filepath = "./import/"

# 2. Nom du fichier des tops DEPART (temps "humain") (doit être dans le dossier import_csv_filepath)
human_start_filename = "human_start_dates.csv"

# 3. Nom du fichier des tops STOP (temps "humain") (dans le dossier import_csv_filepath)
human_stop_filename = "human_stop_dates.csv"

# 4. Chemin vers le répertoire contenant les fichiers des tops DEPART et STOP en temps POSIX
export_timestamp_path = "./export/"

# ===========FIN DES MODIFICATIONS UTILISATEUR   ===================


def convert_date_to_timestamp(date_filepath: str, timestamp_filepath: str) -> None:
    timestamps_export = []

    with open(date_filepath, encoding='UTF-8') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            year, month, day, hour, minutes, seconds = int(row['AAAA']), int(
                row['MM']), int(row['JJ']), int(row['h']), int(row['min']), int(row['secs'])
            import_date = datetime.datetime(
                year, month, day, hour, minutes, seconds)
            timestamps_export.append(str(int(import_date.timestamp())))

    with open(timestamp_filepath, 'w', encoding='UTF-8') as export_file:
        export_file.write("[" + "\n")
        for ts in timestamps_export:
            export_file.write(ts + ",\n")
        export_file.write("]")


timestamp_start_filename = "timestamp_start.txt"
timestamp_stop_filename = "timestamp_stop.txt"

human_start_filepath = os.path.join(import_csv_filepath, human_start_filename)
human_stop_filepath = os.path.join(import_csv_filepath, human_stop_filename)

timestamp_start_filepath = os.path.join(
    export_timestamp_path, timestamp_start_filename)
timestamp_stop_filepath = os.path.join(
    export_timestamp_path, timestamp_stop_filename)

convert_date_to_timestamp(human_start_filepath, timestamp_start_filepath)
convert_date_to_timestamp(human_stop_filepath, timestamp_stop_filepath)
