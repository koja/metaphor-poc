#! /usr/bin/python

import sys
import os
from itertools import dropwhile

if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()
        sys.exit(0)

    if not os.path.isfile(ldmap_filename):
        print("database not found")
        usage()
        sys.exit(0)

    print parse_ldmap( sys.argv[1] )



def usage():
    print("usage : parse_ldmap.py path/to/ldmap_file")



def parse_ldmap(ldmap_filename):
    result = ""
    with open(ldmap_filename, 'r') as ldmap_file:
        not_eof = False
        for line in ldmap_file:
             if line.strip() == "Cross Reference Table":
                 not_eof = True
                 break
        if not not_eof:
            raise ValueError('invalid ldmap format - missing "Cross Reference Table" line')

        line = ldmap_file.next()
        # This line should be empty
        if len(line.strip()) > 0:
            raise ValueError('invalid ldmap format - missing empty line after "Cross Reference Table"')

        line = ldmap_file.next()
        if " ".join( line.split() ) != "Symbol File":
            raise ValueError('invalid ldmap format - missing "Symbol File" line')

        current_symbol = ""
        current_symbol_def = ""
        current_symbol_has_been_used = False

        for line in ldmap_file:
            line_data = line.split()
            if len(line_data) == 1:
                if len(current_symbol) > 0:
                    # not returning symbols that are not explicitly used in final binary
                    if not current_symbol_has_been_used:
                        result += "def " + current_symbol + " " + current_symbol_def + os.linesep
                    result += "use " + " " + current_symbol + " " + line_data[0] + os.linesep
                    current_symbol_has_been_used = True
            elif len(line_data) == 2:
                current_symbol = line_data[0]
                current_symbol_def = line_data[1]
                current_symbol_has_been_used = False
            else:
                # TODO FIXME - filename could potentially contain escaped spaces - would the splitting work?
                raise ValueError('invalid ldmap format - cross reference line contains more than 2 values')
    return result
