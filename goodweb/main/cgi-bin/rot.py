#!/usr/bin/env python3

import cgi
import html


def rot3(text):
    result = []
    for char in text:
        if 'A' <= char <= 'Z':
            result.append(chr((ord(char) - ord('A') + 3) % 26 + ord('A')))
        elif 'a' <= char <= 'z':
            result.append(chr((ord(char) - ord('a') + 3) % 26 + ord('a')))
        else:
            result.append(char)
    return ''.join(result)

form = cgi.FieldStorage()
texte_original = form.getvalue("texte")


texte_encode = rot3(texte_original)

print("Content-Type: text/html\r")
print("\r")
print(f"""
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Résultat ROT-3</title>
    <style>
        body {{
            width: 100%;
            margin: 0;
            text-align: center;
            background-color: #ffffff;
        }}
        .top-bar {{
            width: 100%;
            display: grid;
            grid-auto-flow: column;
            grid-auto-columns: 1fr;
            color: #00ff00;
            border-top: 2px solid black;
            border-bottom: 2px solid black;
        }}
        .top-bar a {{
            padding: 2px;
            border-left: 1px solid black;
            border-right: 1px solid black;
            text-decoration: none;
            color: inherit;
        }}
        .container {{
            margin-top: 50px;
        }}
        .result {{
            font-size: 20px;
            font-weight: bold;
            padding: 10px;
            border: 2px solid black;
            display: inline-block;
            background-color: #f8f8f8;
        }}
        #back-btn {{
            margin-top: 20px;
            padding: 10px 20px;
            font-size: 16px;
            background-color: #ff0000;
            color: white;
            border: none;
            cursor: pointer;
        }}
        #back-btn:hover {{
            background-color: #cc0000;
        }}
    </style>
</head>
<body>

    <div class="top-bar">
        <a href="/">Accueil</a>
        <a href="/options">Options</a>
        <a href="/help">Aide</a>
    </div>

    <div class="container">
        <h1>Texte encodé en ROT-3</h1>
        <p class="result">{html.escape(texte_encode)}</p><br>
        <button id="back-btn" onclick="window.history.back();">Retour</button>
    </div>

</body>
</html>
""")
