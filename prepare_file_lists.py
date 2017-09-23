#! /usr/bin/python

import os

def get_list_of_files(root, suffix):
    lst=[]
    for root, dirs, files in os.walk(root):
        for file in files:
            if file.endswith(suffix):
                 lst.append( os.path.abspath( os.path.join(root, file) ) )
    return lst



def create_file_lists(project_root):
    list_of_ast_files = get_list_of_files(project_root, ".ast")
    list_of_ldmap_files = get_list_of_files(project_root, ".ldmap")
    list_of_crmap_files = []
    for ldmap_file in list_of_ldmap_files:
        crmap_filename = ldmap_file + ".crmap"
        crmap_file = open(crmap_filename, "w")
        crmap_file.write( parse_ldmap.parse_ldmap( ldmap_file ) )
        crmap_file.close()

        list_of_crmap_files.append(crmap_filename)

    ast_list_file = open("ast.list", 'w')
    for ast_file in list_of_ast_files:
        ast_list_file.write( ast_file + os.linesep )
    ast_list_file.close()

    crmap_list_file = open("crmap.list", 'w')
    for crmap_file in list_of_crmap_files:
        crmap_list_file.write( crmap_file + os.linesep )
    crmap_list_file.close()



def usage():
    print("usage : prepare_input_data.py path/to/project/sources")



import sys
import parse_ldmap

if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()
        sys.exit(0)

    create_file_lists(sys.argv[1])
