import logging
import logging.handlers
import boto.sdb
import json


from wsgiref.simple_server import make_server
from cgi import parse_qs, escape


# Create logger
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

# Handler 
LOG_FILE = '/tmp/sample-app.log'
handler = logging.handlers.RotatingFileHandler(LOG_FILE, maxBytes=1048576, backupCount=5)
handler.setLevel(logging.INFO)

# Formatter
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# Add Formatter to Handler
handler.setFormatter(formatter)

# add Handler to Logger
logger.addHandler(handler)

def application(environ, start_response):
	path    = environ['PATH_INFO']
	method  = environ['REQUEST_METHOD']
	if method == 'POST':
		try:
			request_body_size = int(environ['CONTENT_LENGTH'])
			request_body = environ['wsgi.input'].read(request_body_size)
			logger.info("Received message from worker: %s" % request_body)
		except (TypeError, ValueError):
			logger.warning('Error retrieving request body for async work.')
	status = '200 OK'
	headers = [('Content-type', 'text/html')]
	d = parse_qs(environ['QUERY_STRING'])
	conn = boto.sdb.connect_to_region ("us-east-1", aws_access_key_id = "AKIAI7MU2H6XHXFDSX6Q", aws_secret_access_key = "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")
	domain = conn.get_domain ("artworks")
	where = d["query"][0]
	query = "select * from `artworks` " + where + " and Id is not null order by Id asc"
	response = domain.select (query)
	rs = ""
	for i in response:
		rs += "<p>Id: " + i.name + "</br>"
		for j in i:
			if i[j] and j != u"Id":
				rs += "&emsp;" + j.encode ("utf-8") + ": " + i[j].encode ("utf-8") + "</br>"
		rs += "</br>"
	rs = rs.encode ("ascii", "ignore")
	start_response(status, headers)
	return [rs]
	


if __name__ == '__main__':
	httpd = make_server('', 8000, application)
	print "Serving on port 8000..."
	httpd.serve_forever()
