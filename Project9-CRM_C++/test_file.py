#!/usr/bin/python

import os
import re
try:
    import colorama as color
    color.init(autoreset=True)
except:
    print ("Colorama is not installed on this machine.")
    print ("Go to https://pypi.python.org/pypi/colorama to install")

test_passed = True
all_tests_pass = True
    
def VerifyOutput():
    global test_passed
    global all_tests_pass
    expected_file = open("expected.txt", "r")
    output_file = open("Output.txt", "r")
    while True:
        expected_line = getNextLine(expected_file)
        output_line = getNextLine(output_file)

        if re.search(r"/\*+", output_line):
            #print "Skipping after: " + output_line.strip()
            m_obj = re.search(r"end of test *(\d+)", output_line)
            if m_obj and test_passed:
                print "Test " + str(m_obj.group(1)) + \
                      color.Fore.GREEN + color.Style.BRIGHT + " passed"
                output_line = getNextLine(output_file)
            elif m_obj and not test_passed:
                print "Test " + str(m_obj.group(1)) + \
                      color.Fore.RED + color.Style.BRIGHT + " failed"
                test_passed = True
                output_line = getNextLine(output_file)
            else:
                output_line = skip_end_of_test(output_file)

        if expected_line != output_line:
            print "Expected: " + expected_line
            print "Output: " + output_line
            test_passed = False
            all_tests_pass = False
            #assert False

        if expected_line == '':
            print "End of expected file"
            break;

    if all_tests_pass:
        print color.Back.GREEN + color.Fore.BLUE + "Tests pass"
    else:
        print color.Back.RED + "Tests fail"
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
    global test_passed
    finished = False
    line = ''
    while not finished:
        line = f.readline()
        m_obj = re.search(r"end of test *(\d+)", line)
        if m_obj and test_passed:
            finished = True
            print "Test " + str(m_obj.group(1)) + \
                  color.Fore.GREEN + color.Style.BRIGHT + " passed"
        elif m_obj and not test_passed:
            finished = True
            print "Test " + str(m_obj.group(1)) + \
                  color.Fore.RED + color.Style.BRIGHT + " failed"
            test_passed = True
    line = getNextLine(f)
    return line

if __name__ == "__main__":
    VerifyOutput()
    os.system("pause")
