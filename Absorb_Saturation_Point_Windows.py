import csv
import os
import shutil


def absorb_saturation_point_windows(data_src_path, data_dest_file):

	lists = []
	FinalTable = []
	Degree_List = [4, 6, 8, 10, 12, 14]
	# FinalTable.append(xrange(-1,100))
	file_name = 'stat_result.csv'

	for degree in Degree_List:
		lists.append(degree)
		for graph_number in range(100):

			stat_reslut_file = data_src_path + '\\AD' + str(degree) + '\\' + 'G' + str(graph_number)\
			 + '\\' + 'sim_result\\' + file_name

			if os.path.isfile(stat_reslut_file):
				# print stat_reslut_file
				print  '\\AD' + str(degree) + '\\' + 'G' + str(graph_number)
				with open(stat_reslut_file,'rb') as f:
					reader = csv.reader(f)
					lastline = reader.next()
					for line in reader:
						lastline = line

					lists.append(float(lastline[0]))

				f.close()

		FinalTable.append(list(lists))
		del lists[:]


	myfile = open(data_dest_file, 'wb+')
	wr = csv.writer(myfile)
	wr.writerows(FinalTable)
	myfile.close()

def copy_stat_file(data_src_path, data_dest_file):

	Degree_List = [4, 6, 8, 10, 12, 14]
	# FinalTable.append(xrange(-1,100))
	file_name = 'stat_result.csv'

	for degree in Degree_List:

		for graph_number in range(100):

			stat_reslut_file = data_src_path + '\\AD' + str(degree) + '\\' + 'G' + str(graph_number)\
			 + '\\' + 'sim_result\\' + file_name

			dest_stat_reslut_folder = data_dest_file +  '\\AD' + str(degree) + '\\' + 'G' + str(graph_number)

			if os.path.isfile(stat_reslut_file):
				if not os.path.exists(dest_stat_reslut_folder):
					os.makedirs(dest_stat_reslut_folder)

				shutil.copy(stat_reslut_file, dest_stat_reslut_folder)




if __name__ == '__main__':


	data_folder = 'C:\\Users\\chenyua\\OPNET_Project\\Yu_Chen_SCB'

	
	data_dest_file_scb = "C:\\Users\\chenyua\\OPNET_Project\\stat_data" + '\\Saturation_Point_SCB.csv'
	data_dest_folder_scb = "C:\\Users\\chenyua\\OPNET_Project\\stat_data"
	# data_dest_file_eda = "/Users/chenyu/Workspace/Python/MS_Project/Stat_Result" + '/SP_EDA.csv'
	# data_dest_file_ud = "/Users/chenyu/Workspace/Python/MS_Project/Stat_Result" + '/SP_UD_BFS.csv'


	absorb_saturation_point_windows(data_folder, data_dest_file_scb)
	copy_stat_file(data_folder, data_dest_folder_scb)
































	