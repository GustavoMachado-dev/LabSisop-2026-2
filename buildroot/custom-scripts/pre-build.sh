#!/bin/sh

CUSTOM_DIR=$BASE_DIR/../custom-scripts

# Configuracao de rede do tutorial 1.2
cp $CUSTOM_DIR/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

# Aplicacao systeminfo (servidor REST na porta 8080)
cp $CUSTOM_DIR/systeminfo $BASE_DIR/target/usr/bin/systeminfo
chmod +x $BASE_DIR/target/usr/bin/systeminfo

# Inicializacao automatica do systeminfo no boot
cp $CUSTOM_DIR/S99systeminfo $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S99systeminfo
