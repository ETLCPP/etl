  
import shutil
import os

# Get the current Path of the script
arduino_dir = os.path.dirname(__file__)
print('arduino_dir         ', arduino_dir)

etl_dir = os.path.dirname(arduino_dir)
print('etl_dir             ', etl_dir)

etl_include_dir = os.path.join(etl_dir, 'include')
print('etl_include_dir     ', etl_include_dir)

common_dir = os.path.dirname(etl_dir)
print('common_dir          ', common_dir)

etl_arduino_dir = os.path.join(common_dir, 'etl-arduino')
print('etl_arduino_dir     ', etl_arduino_dir)

etl_arduino_src_dir = os.path.join(etl_arduino_dir, 'src')
print('etl_arduino_src_dir ', etl_arduino_src_dir)

print('')

# Copy the library properties
filename    = 'library.properties'
source      = os.path.join(etl_dir, filename)
destination = os.path.join(etl_arduino_dir, filename)
print('source      = ', source)
print('destination = ', destination)
shutil.copyfile(source, destination)

# Copy the Arduino ETL header
filename    = 'Embedded_Template_Library.h'
source      = os.path.join(arduino_dir, filename)
destination = os.path.join(etl_arduino_src_dir, filename)
print('source      = ', source)
print('destination = ', destination)
shutil.copyfile(source, destination)

# Copy the ETL headers
source      = etl_include_dir
destination = etl_arduino_src_dir
print('source      = ', source)
print('destination = ', destination)
shutil.copytree(source, destination, dirs_exist_ok = True)
