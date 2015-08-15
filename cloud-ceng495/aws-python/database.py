import boto.sdb
conn = boto.sdb.connect_to_region ("us-east-1", aws_access_key_id = "AKIAI7MU2H6XHXFDSX6Q", aws_secret_access_key = "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")

conn.delete_domain ("artworks")
conn.create_domain ("artworks")
domain = conn.get_domain ("artworks")
with open ('data_csv', 'r') as f:
    content = f.read()

content = content.split ("\n")
keys = content[0].split (",")
keys[-1] = keys[-1][:-1]
keys[0] = "Id"
content = content[1:]
content = content[:-1]
for i in content:
	newitem = i.split (",")
	newitem[-1] = newitem[-1][:-1]
	item_name = newitem[0]
	item_attrs = {}
	temp = 0
	while (temp < len (keys)):
		item_attrs[keys[temp]] = newitem[temp]
		temp += 1
	print item_name
	print item_attrs
	domain.put_attributes (item_name, item_attrs)
