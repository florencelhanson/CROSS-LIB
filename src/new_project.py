#!/bin/python


import shutil

import os,sys

if len(sys.argv)<2:
    project_type = "demo"
else:
    if(sys.argv[1]=="game"):
        project_type = "game"
    else:
        project_type = "demo"

if len(sys.argv)<3:
    game_dir = "foo_"+project_type
else:
    game_dir = sys.argv[2]

parent_dir = project_type+"s"

print("New project name: " + game_dir)
print("Project type: " + project_type)

parent_and_game_dir = parent_dir + "/" + game_dir

if not os.path.exists(parent_and_game_dir):
    print("Creating project dir...")
    os.makedirs(parent_and_game_dir)

source_path = "./template_projects/"+project_type+"_code.template"
print("source_path: " + source_path)
dest_path = parent_and_game_dir
print("dest_path: " + dest_path)

if not os.path.exists(dest_path+"/tiles"):
    print("Copying tiles...")
    shutil.copytree(source_path+"/tiles", dest_path+"/tiles")

if not os.path.exists(dest_path+"/generated_assets"):
    print("Create empty generated_assets directory...")
    os.makedirs(dest_path+"/generated_assets")

file_names = ["char_tiles.h", "images.h", "main.c"]


for file_name in file_names:
    print("Copying file_name: " + file_name)
    shutil.copy(source_path+"/"+file_name, dest_path)

templated_makefile_path = "./template_projects"

shutil.copy(templated_makefile_path+"/"+"Makefile_"+project_type+".template", "./Makefile."+game_dir)

#read input file
fin = open("./Makefile."+game_dir, "rt")
#read file contents to string
data = fin.read()
#replace all occurrences of the required string
data = data.replace('_GAME_NAME_', game_dir)
data = data.replace('_PARENT_DIR_', parent_dir)
#close the input file
fin.close()
#open the input file in write mode
fin = open("./Makefile."+game_dir, "wt")
#overrite the input file with the resulting data
fin.write(data)
#close the file
fin.close()



    