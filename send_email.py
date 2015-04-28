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