from shutil import copyfile
import glob, os
import sys

SOURCE_ROOT = '.\\src'
FILES = []
DEPENDENCIES_ROOT = '.\\libs'
DLLS = []

SUBDIRS = ['core', 'math', 'network', 'util', 'graphics', 'audio']
BUILD_TYPE = 'Debug'

# function to help get all required DLLs recursively
def get_dlls(root):
    """
    Sets the list of required DLLs and copies it the the library folder
    """
    for item in os.listdir(root):
        if os.path.isfile(os.path.join(root, item)):
            if item.endswith('.dll'):
                DLLS.append(os.path.join(root, item))
        else:
            get_dlls(os.path.join(root, item))

if __name__ == "__main__":
    # get include files in the root src folder
    for rootfile in os.listdir(SOURCE_ROOT):
        if rootfile.endswith('.hpp'):
            FILES.append(os.path.join(SOURCE_ROOT, rootfile))
    # get include files in second level folders
    for subdir in SUBDIRS:
        for subfile in os.listdir(os.path.join(SOURCE_ROOT, subdir)):
            if subfile.endswith('.hpp'):
                FILES.append(os.path.join(subdir, subfile))
    # make sure target directories exist before trying to copy
    if not os.path.exists('.\\engine'):
        os.makedirs('.\\engine')
    if not os.path.exists('.\\engine\\includes'):
        os.makedirs('.\\engine\\includes')
    if not os.path.exists('.\\engine\\lib'):
        os.makedirs('.\\engine\\lib')
    if not os.path.exists('.\\engine\\dll'):
        os.makedirs('.\\engine\\dll')
    for subdir in SUBDIRS:
        target = os.path.join('.\\engine\\includes', subdir)
        if not os.path.exists(target):
            os.makedirs(target)
    # copy the files to the new folder
    for _file in FILES:
        src = os.path.join(SOURCE_ROOT, _file)
        dest = os.path.join('.\\engine\\includes', _file)
        copyfile(src, dest)
    # create a top level engine header to include everything
    with open('.\\engine\\includes\\QcEngine.hpp', 'w') as header:
        header.write('#pragma once\n')
        for _file in FILES:
            header.write('#include\"' + _file + '\"\n')
    # Use different build types depending on command line flags
    if len(sys.argv) > 1:
        if sys.argv[1][0] == 'd' or sys.argv[1][0] == 'D':
            BUILD_TYPE = 'Debug'
        if sys.argv[1][0] == 'r' or sys.argv[1][0] == 'R':
            BUILD_TYPE = 'Release'
    # Copy the Engine library files
    copyfile(os.path.join(BUILD_TYPE, 'QcEngine.lib'), '.\\engine\\lib\\QcEngine.lib')
    copyfile(os.path.join(BUILD_TYPE, 'QcEngine.pdb'), '.\\engine\\lib\\QcEngine.pdb')
    # Copy the required DLL files
    get_dlls(DEPENDENCIES_ROOT)
    for dll in DLLS:
        file_name = os.path.basename(dll)
        copyfile(dll, '.\\engine\\dll\\' + file_name)
