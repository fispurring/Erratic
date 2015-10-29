cerName=$(basename $1 .cer) 
keyName=$(basename $2 .p12)
openssl x509 -in $1 -inform der -out "$cerName"".pem"
openssl pkcs12 -nocerts -out "$keyName"".pem" -in $2
cat "$cerName"".pem" "$keyName"".pem" > $3