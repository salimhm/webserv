#!/usr/bin/python3
import cgi
import os
import shutil

form = cgi.FieldStorage()

upload_dir = './upload'
os.makedirs(upload_dir, exist_ok=True)

for key in form.keys():
    items = form[key]
    if isinstance(items, list):
        for item in items:
            if item.filename:
                filename = os.path.basename(item.filename)
                file_path = os.path.join(upload_dir, filename)
                with open(file_path, 'wb') as out_file:
                    shutil.copyfileobj(item.file, out_file)
                print(f"<h2>{filename} uploaded successfully.</h2>")
            else:
                print("<h2>No file selected.</h2>")
    else:
        if items.filename:
            filename = os.path.basename(items.filename)
            file_path = os.path.join(upload_dir, filename)
            with open(file_path, 'wb') as out_file:
                shutil.copyfileobj(items.file, out_file)
            print(f"<h2>{filename} uploaded successfully.</h2>")
# while True:
#     pass