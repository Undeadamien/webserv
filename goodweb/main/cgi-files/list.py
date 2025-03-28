#!/usr/bin/env python3

import os
import cgi
import cgitb

cgitb.enable()  # Pour afficher les erreurs dans le navigateur

def generate_html(directory_path):
    try:
        entries = os.listdir(directory_path)
    except Exception as e:
        entries = []
        error_message = f"Erreur lors de la lecture du répertoire : {e}"

    html_content = f"""
    <!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Liste des fichiers</title>
        <style>
            body {{ font-family: Arial, sans-serif; }}
            ul {{ list-style-type: none; padding: 0; }}
            li {{ margin: 5px 0; }}
            a {{ text-decoration: none; color: #007BFF; }}
            a:hover {{ text-decoration: underline; }}
        </style>
    </head>
    <body>
        <h1>Contenu du répertoire : {directory_path}</h1>
        <ul>
    """

    for entry in entries:
        entry_path = os.path.join(directory_path, entry)

        # Vérifier si le fichier n'a pas d'extension et le traiter comme un fichier .txt
        if os.path.isdir(entry_path):
            html_content += f'<li><a href="?path={entry_path}">📁 {entry}</a></li>'
        else:
            # Si c'est un fichier sans extension, traiter comme un fichier .txt
            if '.' not in entry:
                entry = entry + '.txt'

            html_content += f'<li><a href="{entry_path}" target="_blank">📄 {entry}</a></li>'

    html_content += """
        </ul>
    </body>
    </html>
    """

    return html_content

print("Content-Type: text/html\r")
print("\r")

form = cgi.FieldStorage()
path = form.getvalue("path", "goodweb/list")

print(generate_html(path))
