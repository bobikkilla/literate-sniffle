docker run -d -p 81:81 -p 443:443 -p 80:80 --name helloworld_container nginx_import

docker cp server/nginx.conf helloworld_container:/etc/nginx/nginx.conf

docker cp server/mini_server.c helloworld_container:/mini_server.c

docker cp server/run.sh helloworld_container:/run.sh

docker exec helloworld_container chmod +x run.sh

docker exec -it helloworld_container /bin/bash /run.sh

docker exec helloworld_container nginx -s reload