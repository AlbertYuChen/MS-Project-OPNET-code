from __future__ import division
import subprocess
import os
import glob
import shutil
from subprocess import check_output
import csv
from time import strftime,localtime
import os.path
from send_email import send_email


project_name = "WH_PJ"
scenario_name = "SCN_SIM"
stat_result_file = "stat_result.csv"

ef_file = project_name + "-" + scenario_name + "-DES-1"
ov_file = project_name + "-" + scenario_name + "-DES-1"

WORM_TIME = 0.0000018

def write_log(input_string):
	os.chdir('C:\\Users\\chenyua\\OPNET_Project\\WH_G8x8')
	sys_log_file = open("sys_log.txt", 'a')
	print >> sys_log_file, input_string
	sys_log_file.close()

def write_ef_file(sim_folder, Worm_Gen_Rate):
	os.chdir(sim_folder + "\\" + project_name + ".project")

	target_ef_file = open(ef_file + ".ef", 'w')

	print >> target_ef_file, '"_locale":	"C"'
	print >> target_ef_file, '"anim_view":	"false"'
	print >> target_ef_file, '"Campus Network.*.Worm_Gen_Len":	"%s"' % 200
	print >> target_ef_file, '"Campus Network.*.Worm_Gen_Rate":	"%f"' % Worm_Gen_Rate
	print >> target_ef_file, '"comp_trace_info":	"kernel-based"'
	print >> target_ef_file, '"default_site_position_cache_time_granularity":	"0.0"'
	print >> target_ef_file, '"des.log_severity_filter":	"-1"'
	print >> target_ef_file, '"duration":	"3,600.0"'
	print >> target_ef_file, '"log_endsim_perf":	"true"'
	print >> target_ef_file, '"log_file":	"%s"' % ef_file
	print >> target_ef_file, '"max_log_entries":	"200"'
	print >> target_ef_file, '"num_collect_values":	"100"'
	print >> target_ef_file, '"ot_file":	"%s"' % ef_file
	print >> target_ef_file, '"ov_file":	"%s"' % ov_file
	print >> target_ef_file, '"parallel_sim.event_execution_time_window":	"0.0"'
	print >> target_ef_file, '"parallel_sim.mem_balance_interval":	"100,000"'
	print >> target_ef_file, '"parallel_sim.num_processors":	"1"'
	print >> target_ef_file, '"probe":	"WH_PJ-SCN_SIM"'
	print >> target_ef_file, '"realtime_ratio":	"0.0"'
	print >> target_ef_file, '"script_mode":	"none"'
	print >> target_ef_file, '"seed":	"128"'
	print >> target_ef_file, '"sim_packet_sharing":	"conservative"'
	print >> target_ef_file, '"sim_time_quantum":	"0.0"'
	print >> target_ef_file, '"site_position_cache_size":	"3"'
	print >> target_ef_file, '"tmm_simulate":	"false"'
	print >> target_ef_file, '"update_interval":	"50,000,000"'
	print >> target_ef_file, '"verbose_cell_size_report":	"false"'
	print >> target_ef_file, '"verbose_event_report":	"false"'
	print >> target_ef_file, '"verbose_event_timing_report":	"false"'
	print >> target_ef_file, '"verbose_load":	"true"'
	print >> target_ef_file, '"verbose_packet_report":	"true"'
	print >> target_ef_file, '"verbose_parallel_speedup_report":	"false"'
	print >> target_ef_file, '"verbose_sim":	"true"'

	target_ef_file.close()

def read_last_sim_saturation_point(sim_folder):

	target_stat_result_file = sim_folder + "\\" + stat_result_file

	if os.path.isfile(target_stat_result_file):
		with open(target_stat_result_file,'rb') as f:
			reader = csv.reader(f)
			lastline = reader.next()
			for line in reader:
				lastline = line

		write_log(lastline)
		return float(lastline[1])
	else:
		# print "ERROR: there is no %s" % target_stat_result_file
		return -2

def opnet_run_simulation():

	op_runsim_prg = "C:\\OPNET\\17.1.A\\sys\\pc_intel_win32\\bin\\op_runsim "

	args = []

	env_db = "-env_db opnet-17-1.prefs"
	args.append(env_db)

	recompile_everything = "-c false"
	args.append(recompile_everything)

	kernel_type = "-kernel_type optimized"
	args.append(kernel_type)

	debug = "-debug false"
	args.append(debug)

	nopromot = "-nopromot TRUE"
	args.append(nopromot)

	scenario_file = "-net_name " + project_name + "-" + scenario_name
	args.append(scenario_file)

	args.append("-ef " + ef_file)

	arguments = " ".join(args)

	# write_log(op_runsim_prg)
	# write_log(arguments)

	os.system(op_runsim_prg + arguments)


def copy_sim_files(data_folder, sim_folder):
	src_files = os.listdir(data_folder)
	
	for file_name in src_files:
		full_file_name = os.path.join(data_folder, file_name)
		if (os.path.isfile(full_file_name)):
			shutil.copy(full_file_name, sim_folder)

	os.chdir(sim_folder)
	for file1 in glob.glob("*.nt.m"):
		new_net_file = project_name + "-" + scenario_name + ".nt.m"
		os.rename(file1, new_net_file)
		shutil.copy(new_net_file, sim_folder + "\\" + project_name + ".project")
		os.remove(new_net_file)
		break

def backup_sim_files(data_folder, sim_folder, Ave_Degree, Graph_Number, Worm_Gen_Rate):


	proj_folder = sim_folder + "\\" + project_name + ".project"
	my_ov_file = proj_folder + "\\" + ov_file + ".ov"
	my_result = sim_folder + "\\" + stat_result_file
	new_ov_name = "AD" + `Ave_Degree` + "-G" + `Graph_Number` + "-WGR-" +\
		`Worm_Gen_Rate` + "-" + ov_file + "-" + strftime("%Y-%m-%d_%H-%M-%S", localtime()) +".ov"

	os.rename(my_ov_file, new_ov_name)

	result_folder = data_folder + "\\sim_result"

	if not os.path.exists(result_folder):
		os.makedirs(result_folder)

	shutil.move(new_ov_name, result_folder) # be careful of the name confliction
	shutil.copy(my_result, result_folder) # will overwrite the target file



# if __name__ == "__main__":

def run_sim_once(Ave_Degree, Graph_Number):

	# Ave_Degree = 4
	# Graph_Number = 0

	data_folder = 'C:\\Users\\chenyua\\OPNET_Project\\UPDOWN\\AD' + `Ave_Degree` + '\\G' + `Graph_Number`
	# data_folder = 'C:\\Users\\chenyua\\OPNET_Project\\Yu_Chen_SCB\\AD' + `Ave_Degree` + '\\G' + `Graph_Number`
	sim_folder = 'C:\\Users\\chenyua\\OPNET_Project\\WH_G8x8'
	

	Worm_Gen_Rate = 1.0
	sub_simulation_counter = 0
	last_sim_saturation_point = 0.0
	WGR_Interval_Left = 1.0
	WGR_Interval_Right = 1.0

	write_log("\n\n+++++++++++++++++ start simulation of UD new graph  AD%d  G%d  +++++++++++++++++" % (Ave_Degree, Graph_Number))
	write_log(strftime("%Y-%m-%d_%H-%M-%S", localtime())) 

	copy_sim_files(data_folder, sim_folder)

	# if (os.path.isfile(sim_folder + "\\" + stat_result_file)):
	# 	os.remove(sim_folder + "\\" + stat_result_file)

	# empirical_Worm_Gen_Rate = {4:[1, 100, 1000, 105000, 120000], 6:[1.8, 180, 18000, 180000, 230000],\
	#  8:[2.4, 240, 24000, 235000, 270000],12:[2.6, 260, 26000, 260000, 320000]}

	empirical_Worm_Gen_Rate = {4:[1, 100, 1000, 60000, 100000], 6:[1.8, 180, 18000, 120000, 230000],\
	 8:[2.4, 240, 24000, 180000, 270000],12:[2.6, 260, 26000, 220000, 320000]}

	WGR_Interval_Left = empirical_Worm_Gen_Rate[Ave_Degree][3] # last basic case
	WGR_Interval_Right = empirical_Worm_Gen_Rate[Ave_Degree][4]	# right most guess

	while True:

		if last_sim_saturation_point > 10 * WORM_TIME:
			break

		if WGR_Interval_Right - WGR_Interval_Left < 100:
			break

		if sub_simulation_counter <= 3:
			Worm_Gen_Rate = empirical_Worm_Gen_Rate[Ave_Degree][sub_simulation_counter]
		elif sub_simulation_counter == 4:
			Worm_Gen_Rate = (WGR_Interval_Left + WGR_Interval_Right) / 2
		else:
			if last_sim_saturation_point == -1:
				WGR_Interval_Right = Worm_Gen_Rate
				Worm_Gen_Rate = (WGR_Interval_Left + WGR_Interval_Right) / 2
			else:
				WGR_Interval_Left = Worm_Gen_Rate
				Worm_Gen_Rate = (WGR_Interval_Left + WGR_Interval_Right) / 2

		# Worm_Gen_Rate = 150000

		write_log("-------------------- %d  running on Worm_Gen_Rate: %f  --------------------"  % (sub_simulation_counter, Worm_Gen_Rate)) 
		write_log(strftime("%Y-%m-%d_%H-%M-%S", localtime())) 

		write_ef_file(sim_folder, Worm_Gen_Rate)

		opnet_run_simulation()

		backup_sim_files(data_folder, sim_folder, Ave_Degree, Graph_Number, Worm_Gen_Rate)

		sub_simulation_counter = sub_simulation_counter + 1

		# decide the Worm_Gen_Rate
		last_sim_saturation_point = read_last_sim_saturation_point(sim_folder)

		if sub_simulation_counter > 25:
			break

		send_email()


	last_sim_saturation_point = 0.0
	os.remove(sim_folder + "\\" + stat_result_file)





if __name__ == "__main__":

	# Ave_Degree = 8
	# Graph_Number = 0
	# run_sim_once(Ave_Degree, Graph_Number)


	# for Ave_Degree in [4, 6, 8, 10, 12, 14]:
	for Ave_Degree in [6, 8]:
		for Graph_Number in xrange(1, 2):
			run_sim_once(Ave_Degree, Graph_Number)

	for Graph_Number in xrange(2, 8):
		for Ave_Degree in [4, 6, 8]:
			run_sim_once(Ave_Degree, Graph_Number)
			

