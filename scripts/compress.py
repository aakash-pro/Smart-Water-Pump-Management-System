import os
import gzip
import shutil

# Paths
data_dir = "data"
files_to_compress = ["index.html", "style.css", "app.js"]

# Ensure data directory exists
if not os.path.exists(data_dir):
    os.makedirs(data_dir)

# Compress each file
for filename in files_to_compress:
    filepath = os.path.join(data_dir, filename)
    gz_filepath = filepath + ".gz"
    
    if os.path.exists(filepath):
        print(f"Compressing {filename}...")
        with open(filepath, 'rb') as f_in:
            with gzip.open(gz_filepath, 'wb', compresslevel=9) as f_out:
                shutil.copyfileobj(f_in, f_out)
        print(f"  Created {filename}.gz")
    else:
        print(f"Warning: {filename} not found")

print("Compression complete!")
