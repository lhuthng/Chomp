import cppyy

def load_cpp_scripts(debug=False):
    import subprocess, os

    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    include_dir = 'include'
    src_dir = 'src'

    scripts = [
        'board', 'iterator', 'evidence', 'pattern'
    ]
    script_locations = ' '.join(map(lambda scr: f'{src_dir}/{scr}.cpp', scripts))
    command = f'g++ -std=c++17 -shared -o shared.so -I{include_dir} {script_locations} -fPIC'
    process = subprocess.run(command, capture_output=True, text=True, shell=True)
    if debug: print(f'Output: {process.stdout}\nError: {process.stderr}')
    cppyy.load_library("shared.so")
    for script in scripts:
        cppyy.include(f'{include_dir}/{script}.h')

    return cppyy.gbl

def cpp_iterator(iterator):
    iterator.init()
    while iterator.hasNext():
        yield tuple(iterator.getNext())