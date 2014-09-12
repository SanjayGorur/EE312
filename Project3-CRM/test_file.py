#!/usr/bin/python

import os
import re
try:
    import colorama as color
    color.init(autoreset=True)
except:
    print ("Colorama is not installed on this machine.")
    print ("Go to https://pypi.python.org/pypi/colorama to install")
    
def VerifyOutput():
    expected_file = open("expected.txt", "r")
    output_file = open("Output.txt", "r")
    while True:
        expected_line = getNextLine(expected_file)
        output_line = getNextLine(output_file)

        if re.search(r"/\*+", output_line):
            #print "Skipping after: " + output_line.strip()
            m_obj = re.search(r"end of test *(\d+)", output_line)
            if m_obj:
                print "Test " + str(m_obj.group(1)) + \
                      color.Fore.GREEN + color.Style.BRIGHT + " passed"
                output_line = getNextLine(output_file)
            else:
                output_line = skip_end_of_test(output_file)

        if expected_line != output_line:
            print "Expected: " + expected_line
            print "Output: " + output_line
            assert False

        if expected_line == '':
            print "End of expected file"
            break;

    print color.Back.GREEN + color.Fore.BLUE + "Tests pass"
    expected_file.close()
    output_file.close()

def getNextLine(f):
    finished = False
    while not finished:
        line = f.readline()
        if not line.isspace():
            finished = True

    return line

def skip_end_of_test(f):
    finished = False
    line = ''
    while not finished:
        line = f.readline()
        m_obj = re.search(r"end of test *(\d+)", line)
        if m_obj:
            finished = True
            print "Test " + str(m_obj.group(1)) + \
                  color.Fore.GREEN + color.Style.BRIGHT + " passed"
    line = getNextLine(f)
    return line

if __name__ == "__main__":
    VerifyOutput()
    os.system("pause")
