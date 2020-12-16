  
import shutil
import os

# https://arduino.github.io/arduino-cli/latest/library-specification/

#
# Some Constants
library_name = 'Embedded_Template_Library'

#
# Get the current Path of the script
dirname = os.path.dirname(__file__)
dest = os.path.join(dirname, library_name)
print('The destination folder is ', dest)

#
# Copy the ETL to the Arduino-Library Folder
src = os.path.join(dirname, 'include')
print('The source folder is ', src)
print('The destination folder is ', dest)
shutil.copytree(src, os.path.join(dest, 'src'),  dirs_exist_ok=True)
print('Done adding ETL...')

#
# Add files from the arduino folder
filename = 'etl.h'
src = os.path.join(os.path.join(dirname, 'arduino'), filename)
print('The source is ', src)
print('The destination is ', os.path.join(os.path.join(dest, 'src'), filename))
shutil.copyfile(src, os.path.join(os.path.join(dest, 'src'), filename))
print('Done adding Arduino...')

#
# Add any additional file (library.properties)
filename = 'library.properties'
shutil.copyfile(os.path.join(dirname, filename), os.path.join(dest, filename))
print('Done adding additional files...')

#
# Zip the Folder to be Arduino-Compatible
print('Make archive...')
shutil.make_archive(os.path.join(os.path.join(dirname, 'arduino'), library_name), 'zip', dest)

#
# Remove temporary directory
print('Removing temporary files...')
shutil.rmtree(dest)
