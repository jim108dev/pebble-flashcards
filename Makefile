
# Connection
COMM_PATH := ~/github/pebble-pc-communication-example/host_python
VENV_PATH := $(COMM_PATH)/venv/bin
PYTHON2 := $(VENV_PATH)/python

DOWNLOAD := $(PYTHON2) $(COMM_PATH)/pebble_download.py
UPLOAD := $(PYTHON2) $(COMM_PATH)/pebble_upload.py

CONN_PATH := ./connection
CONN_CONF_WATCH := $(CONN_PATH)/config_watch.ini
CONN_CONF_EMU := $(CONN_PATH)/config_emu.ini

# Evaluation
EVAL_PATH := ./evaluation
VENV_PATH := $(EVAL_PATH)/venv/bin
PYTHON3 := $(VENV_PATH)/python

ALIGN_HISTORY := $(PYTHON3) $(EVAL_PATH)/align_history.py
PREPARE := $(PYTHON3) $(EVAL_PATH)/prepare_next_session.py
MERGE := $(PYTHON3) $(EVAL_PATH)/merge_feedback.py
EVAL_CONF := $(EVAL_PATH)/config.ini


# pebble source
PEBBLE_SRC := ./pebble_app

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

# Evaluation
align_history:
	$(ALIGN_HISTORY) $(EVAL_CONF)

prepare_next_session:
	$(PREPARE) $(EVAL_CONF)

merge_feedback:
	$(MERGE) $(EVAL_CONF)