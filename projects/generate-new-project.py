import sys
import shutil
import os

if len(sys.argv) < 2:
    print("You must provide a name of the project to make")
    exit(-1)

project_name = sys.argv[1]

shutil.copytree('template', project_name)
files = os.listdir(project_name)
for file in files:
    # Read the content of the file and replace template placeholders with project name
    full = project_name + '/' + file
    with open(full, 'r') as current_file:
        file_content = current_file.read()
        modified_content = file_content.replace('$template', project_name)

    # Write new version of the file
    with open(full, 'w') as current_file:
        current_file.write(modified_content)


print('Project created')
