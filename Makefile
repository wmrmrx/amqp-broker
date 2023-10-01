PORT ?= 1234
ACTIVE_CONTAINERS=

build: 
	$(MAKE) -C src/
	cp src/broker ./broker

docker: build
	docker build -t amqp-broker .
	@echo "\033[0;33m"OBS: por algum motivo Ctrl-C não funciona, precisa parar o container com \'Ctrl-PQ\'"\033[0m"
	docker run -it --rm -p $(PORT):$(PORT) -v ./src:/src --name amqp-broker amqp-broker $(PORT)
	docker kill $$(docker ps -aq -f name=amqp-broker)

clean: 
	$(MAKE) -C src/ clean
	rm -f ./broker
	docker rmi -f amqp-broker 2> /dev/null || true
	docker kill $$(docker ps -aq -f name=amqp-broker) 2> /dev/null || true
