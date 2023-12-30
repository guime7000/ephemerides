# Conversion de date au format "humain" vers un timestamp POSIX

La conversion est effectuée lorsque le script python `date_to_conversion.py` est exécuté.

L'exécution du script crée 2 fichiers contenant les timestamps POSIX pour les tops départ et tops stop de l'installation.

La marche à suivre est la suivante:

- dans le répertoire `import`, créer, à l'aide d'un tableur, deux fichiers `csv`, correspondant au modèle `exemple_fichier_dates_humaines.csv`. *ATTENTION : lors de l'export du fichier, le séparateur sélectionné doit être une virgule et pas un point-virgule.*

- modifier le nom des fichiers `human_start_filename` et `human_stop_filename` pour qu'ils correspondent aux noms des fichiers crées précédemment dans le dossier `import` 

- modifier le nom des fichiers d'export des dates au format timestamp POSIX `timestamp_start_filename` et `timestamp_stop_filename`. Les fichiers générés seront créés (par défaut dans le dossier `export`)

- exécuter le script python `date_to_timestamp.py`

Et voilà !
