eval:
	./run_eval.sh "turbine" 6 1
	./run_eval.sh "turbine" 6 2
	./run_eval.sh "turbine" 6 3
	./run_eval.sh "turbine" 6 4
	./run_eval.sh "turbine" 6 5

	./run_eval.sh "delhi" 4 1
	./run_eval.sh "delhi" 4 2
	./run_eval.sh "delhi" 4 3
	./run_eval.sh "delhi" 4 4
	./run_eval.sh "delhi" 4 5

valgrind:
	./run_eval_for_valgrind.sh "turbine" 6 1
	./run_eval_for_valgrind.sh "turbine" 6 2
	./run_eval_for_valgrind.sh "turbine" 6 3
	./run_eval_for_valgrind.sh "turbine" 6 4
	./run_eval_for_valgrind.sh "turbine" 6 5

	./run_eval_for_valgrind.sh "delhi" 4 1
	./run_eval_for_valgrind.sh "delhi" 4 2
	./run_eval_for_valgrind.sh "delhi" 4 3
	./run_eval_for_valgrind.sh "delhi" 4 4
	./run_eval_for_valgrind.sh "delhi" 4 5

