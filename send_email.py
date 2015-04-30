import smtplib
from email.mime.text import MIMEText
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

def send_email():

	SUBJECT = 'finish one graph'
	fromaddr = "albertchenyu@gmail.com"
	# toaddrs = "chenyua@bu.edu"
	toaddrs = fromaddr

	msg = MIMEMultipart()
	msg['Subject'] = SUBJECT
	msg['From'] = fromaddr
	msg['To'] = toaddrs
	msg.attach(MIMEText(file("C:\\Users\\chenyua\\OPNET_Project\\WH_G8x8\\sys_log.txt").read()))

	username = 'albertchenyu'
	password = 'Cy19900516'

	server = smtplib.SMTP('smtp.gmail.com:587')
	server.starttls()
	server.login(username,password)
	server.sendmail(fromaddr, toaddrs, msg.as_string())
	server.quit()







#  C:\OPNET\17.1.A\sys\pc_intel_win32\bin\op_cvov -output_file_path C:\Users\chenyua\OPNET_Project\Yu_Chen_SCB\AD4\G0\sim_result\tt\test.csv -scalar -m C:\Users\chenyua\OPNET_Project\Yu_Chen_SCB\AD4\G0\sim_result\tt\


#  C:\OPNET\17.1.A\sys\pc_intel_win32\bin\op_cvov -output_file_path C:\Users\chenyua\OPNET_Project\Yu_Chen_SCB\AD4\G0\sim_result\tt\test.csv  -vector_header -vector_data -m 