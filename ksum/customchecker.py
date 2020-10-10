def run_custom_checker(t_obj, r_obj):
    r_obj.result = True;
    r_obj.score = 1.0;
    r_obj.message = "Success" if t_obj.testcase_result else "TESTCASE?";
    # Don't print anything to STDOUT in this function
    # Enter your custom checker scoring logic here
    if not t_obj.testcase_result:
        outputyy = t_obj.testcase_output_path
        i = ''
        o = ''
        try:
            i = open(t_obj.testcase_input_path, 'r').read().split()
            o = open(t_obj.testcase_output_path, 'r').read().split()
        except IOError:
            r_obj.result = False
            r_obj.score = 0
            r_obj.message = 'Error reading result file'
            return
        i = list(map(int, i))
        n = i[0]
        k = i[1]
        g = i[2]
        i = i[3:]
        w_x = list(map(int, o))
        count = 0
        if len(w_x) != k:
            r_obj.message = "Not enough values"
            r_obj.score = 0.0
            r_obj.result = False
            return
        while len(w_x) > 0:
            x = w_x[0]
            if x not in i:
                r_obj.message = "You used something out of the list"
                r_obj.score = 0.0
                r_obj.result = False
                return
            count += x
            w_x = w_x[1:]
        if count != g:
            r_obj.message = "You did not add up to g"
            r_obj.score = 0.0
            r_obj.result = False
            return
