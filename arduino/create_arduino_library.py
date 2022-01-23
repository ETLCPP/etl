  
import shutil
import os

# Assumes this folder structure.
# Folder              Variable
# ---------------------------------------
# etl                 etl_dir
#   arduino           arduino_dir
#   include           include_dir
# etl-arduino         etl_arduino_dir
#   src               etl_arduino_src_dir

print('')
print('Copy ETL files to the etl-arduino repository')
print('')

# Get the current path of the script
arduino_dir = os.path.dirname(__file__)
print('arduino_dir              = ', arduino_dir)

# Get the root folder of the ETL
etl_dir = os.path.dirname(arduino_dir)
print('etl_dir                  = ', etl_dir)

# Get the ETL repository folder
include_dir = os.path.join(etl_dir, 'include')
print('include_dir              = ', include_dir)

# Get the ETL arduino_examples folder
arduino_examples_dir = os.path.join(arduino_dir, 'examples')
print('examples_dir             = ', arduino_examples_dir)

# Get the root folder of both repositories
common_dir = os.path.dirname(etl_dir)
print('common_dir               = ', common_dir)

# Get the ETL Arduino repository folder
etl_arduino_dir = os.path.join(common_dir, 'etl-arduino')
print('etl_arduino_dir          = ', etl_arduino_dir)

# Get the ETL Arduino src repository folder
etl_arduino_src_dir = os.path.join(etl_arduino_dir, 'src')
print('etl_arduino_src_dir      = ', etl_arduino_src_dir)

# Get the ETL Arduino examples repository folder
etl_arduino_examples_dir = os.path.join(etl_arduino_dir, 'examples')
print('etl_arduino_examples_dir = ', etl_arduino_examples_dir)

print('')

# Copy the library properties
filename    = 'library.properties'
source      = os.path.join(arduino_dir, filename)
destination = os.path.join(etl_arduino_dir, filename)
print('Copy the library properties')
print('  From :', source)
print('  To   :', destination)
shutil.copyfile(source, destination)

print('')

# Copy the library json
filename    = 'library.json'
source      = os.path.join(arduino_dir, filename)
destination = os.path.join(etl_arduino_dir, filename)
print('Copy the library json')
print('  From :', source)
print('  To   :', destination)
shutil.copyfile(source, destination)

print('')

# Copy the Arduino ETL header
filename    = 'Embedded_Template_Library.h'
source      = os.path.join(arduino_dir, filename)
destination = os.path.join(etl_arduino_src_dir, filename)
print('Copy the Arduino ETL header')
print('  From :', source)
print('  To   :', destination)
shutil.copyfile(source, destination)

print('')

# Copy the ETL headers
source      = include_dir
destination = etl_arduino_src_dir
print('Copy the ETL headers')
print('  From :', source)
print('  To   :', destination)
shutil.copytree(source, destination, dirs_exist_ok = True)

print('')

# Copy the ETL arduino_examples
source      = arduino_examples_dir
destination = etl_arduino_examples_dir
print('Copy the ETL Arduino examples')
print('  From :', source)
print('  To   :', destination)
shutil.copytree(source, destination, dirs_exist_ok = True)
