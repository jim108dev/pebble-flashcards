
# Connection
COMM_PATH := ~/github/pebble-pc-communication-example/host_python
VENV_PATH := $(COMM_PATH)/venv/bin
PYTHON2 := $(VENV_PATH)/python

DOWNLOAD := $(PYTHON2) $(COMM_PATH)/pebble_download.py
UPLOAD := $(PYTHON2) $(COMM_PATH)/pebble_upload.py
TRANSLATE_BINARY := $(PYTHON2) $(COMM_PATH)/translate_binary.py

CONN_PATH := ./connection
CONN_CONF_WATCH := $(CONN_PATH)/config_watch.ini
CONN_CONF_EMU := $(CONN_PATH)/config_emu.ini

PEBBLE_SRC := ./pebble_app

# Evaluation
EVAL_PROJECT := ./evaluation/evaluation.fsproj
EVAL_CONF := -c ./evaluation/Config/Config.yaml
EVAL_RUN := dotnet run --project=$(EVAL_PROJECT) --

EVAL_PREPARE := -u prepare-next-session
EVAL_MERGE := -u merge-feedback


all:
	@echo "Please choose explicitly a target."

init_app:
	cp $(PEBBLE_SRC)/package.example.json $(PEBBLE_SRC)/package.json

# Pebble
install_emu: init_app
	cd pebble_app && pebble build && pebble install --logs --emulator aplite

install_watch: init_app
	cd pebble_app && pebble build && pebble install --serial /dev/rfcomm0

# Connection

upload_emu:
	$(UPLOAD) $(CONN_CONF_EMU)

upload_watch:
	$(UPLOAD) $(CONN_CONF_WATCH)

download_emu:
	$(DOWNLOAD) $(CONN_CONF_EMU)

download_watch:
	$(DOWNLOAD) $(CONN_CONF_WATCH)

translate_binary_emu:
	$(TRANSLATE_BINARY) $(CONN_CONF_EMU)

translate_binary_watch:
	$(TRANSLATE_BINARY) $(CONN_CONF_WATCH)

# Evaluation
prepare_next_session:
	$(EVAL_RUN) $(EVAL_CONF) $(EVAL_PREPARE)

merge_feedback:
	$(EVAL_RUN) $(EVAL_CONF) $(EVAL_MERGE)