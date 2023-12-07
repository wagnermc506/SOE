srcs = api_consumer/*.c database/*.c models/*.c config/*.c recognition/*.c

all: sync service

service: prepare
	gcc $(srcs) service/camera_daemon.c -o build/camera_daemon -lcurl -lcjson -g

sync: prepare
	gcc $(srcs) cmd/sync_prova.c -o build/sync -lcurl -lcjson -g

main: prepare
	gcc $(srcs) main.c -o build/test -lcurl -lcjson -g

prepare:
	mkdir -p build

clean:
	rm -rf build
