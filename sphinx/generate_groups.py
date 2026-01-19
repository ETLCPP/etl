import os
import xml.etree.ElementTree as ET

def generate_group_rst(xml_dir, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Doxygen names group files 'group__*.xml'
    for filename in os.listdir(xml_dir):
        if filename.startswith("group__") and filename.endswith(".xml"):
            tree = ET.parse(os.path.join(xml_dir, filename))
            root = tree.getroot()
            
            # Extract the group ID and Title
            group_id = root.find('.//compounddef').attrib['id']
            group_title = root.find('.//title').text

            # Create the .rst file
            rst_filename = f"{group_id}.rst"
            with open(os.path.join(output_dir, rst_filename), 'w') as f:
                f.write(f"{group_title}\n")
                f.write("=" * len(group_title) + "\n\n")
                f.write(f".. doxygengroup:: {group_id[7:]}\n") # strips 'group__' prefix
                f.write("   :project: Embedded Template Library\n")
                f.write("   :members:\n")

# Example usage
# generate_group_rst('./xml', './api_generated')