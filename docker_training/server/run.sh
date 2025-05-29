apt-get update
apt install spawn-fcgi
apt install -y gcc make libfcgi-dev spawn-fcgi

gcc mini_server.c -o helloworld -lfcgi

spawn-fcgi -p 8080 helloworld