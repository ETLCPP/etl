  
import shutil
import os

# Get the current path of the script
script_dir = os.path.dirname(os.path.abspath(__file__))
  
# Get the root folder of the ETL
etl_dir = os.path.abspath(os.path.join(script_dir, os.pardir))

# Get the ETL repository folder
include_dir = os.path.join(etl_dir, 'include')

# Get the ETL headers folder
headers_dir = os.path.join(include_dir, 'etl')

# Get the Arduino folder
arduino_dir = os.path.join(etl_dir, 'arduino')

# Get the ETL arduino_examples folder
arduino_examples_dir = os.path.join(arduino_dir, 'examples')

# Get the root folder of both repositories
common_dir = os.path.abspath(os.path.join(etl_dir, os.pardir))

# Get the ETL Arduino repository folder
etl_arduino_dir = os.path.join(common_dir, 'etl-arduino')

# Get the ETL Arduino src repository folder
etl_arduino_src_dir = os.path.join(etl_arduino_dir, 'src')

# Get the ETL Arduino examples repository folder
etl_arduino_examples_dir = os.path.join(etl_arduino_dir, 'examples')

major_version = ''
minor_version = ''
patch_version = ''

#------------------------------------------------------------------------------
def get_version():
  version_file = os.path.join(etl_dir, 'version.txt')
  print('')
  print('version_file = ', version_file)
    
  with open(version_file) as f:
    version = f.read().splitlines()

  elements = version[0].split('.', 3)

  return version[0], elements[0], elements[1], elements[2]

#------------------------------------------------------------------------------
def update_version_h():
  print('')
  print('Creating version.h')

  version_h = os.path.join(headers_dir, 'version.h')
  
  with open(version_h) as f:
    text = f.read().splitlines()
  
  search_major = '#define ETL_VERSION_MAJOR '
  search_minor = '#define ETL_VERSION_MINOR '
  search_patch = '#define ETL_VERSION_PATCH '

  length_major = len(search_major)
  length_minor = len(search_minor)
  length_patch = len(search_patch)

  for i in range(len(text) - 1):
    
    index = text[i].find(search_major)
    if index != -1:
      text[i] = text[i][index:length_major] + major_version
      print(text[i])
    
    index = text[i].find(search_minor)
    if index != -1:
      text[i] = text[i][index:length_minor] + minor_version
      print(text[i])

    index = text[i].find(search_patch)
    if index != -1:     
      text[i] = text[i][index:length_patch] + patch_version
      print(text[i])

  with open(version_h, 'w') as f:
    for line in text:
      f.write(line)
      f.write('\n')

#------------------------------------------------------------------------------
def update_library_json(filename):
  print('')
  print('Creating %s' % filename)
  
  with open(filename) as f:
    text = f.read().splitlines()
  
  search = 'version'

  for i in range(len(text) - 1):   
    index = text[i].find(search)
    if index != -1:
      text[i] = '  \"version\": \"' + full_version + '\",'

  with open(filename, 'w') as f:
    for line in text:
      f.write(line)
      f.write('\n')

#------------------------------------------------------------------------------
def update_library_properties(filename):
  print('')
  print('Creating %s' % filename)

  with open(filename, 'r') as f:
    text = f.read().splitlines()
  
  search = 'version'

  for i in range(len(text) - 1):   
    index = text[i].find(search)
    if index != -1:
      text[i] = 'version=' + full_version

  with open(filename, 'w') as f:
    for line in text:
      f.write(line)
      f.write('\n')

#------------------------------------------------------------------------------
def update_versions():
  print('')
  print('Update Versions')

  global full_version
  global major_version
  global minor_version
  global patch_version

  full_version, major_version, minor_version, patch_version = get_version()

  print("Version = %s.%s.%s" % (major_version, minor_version, patch_version ))

  update_version_h()
  
  update_library_json(os.path.join(etl_dir,     'library.json'))

  update_library_properties(os.path.join(etl_dir,     'library.properties'))

#------------------------------------------------------------------------------
if __name__ == "__main__":
  update_versions()
