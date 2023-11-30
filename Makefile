SHELL = /bin/sh
MKDIR_P = mkdir -p

TARGET_DIR = ./build
INCLUDE_DIR = ./include
SOURCE_DIR = ./src
LIB_DIR = ./lib

TARGET_DIRECTORIES = $(TARGET_DIR) $(TARGET_DIR)/modular_arithmetic $(TARGET_DIR)/rsa_c $(TARGET_DIR)/rsa_c/attacks $(TARGET_DIR)/elgamal_c $(TARGET_DIR)/text_message_encoding $(TARGET_DIR)/primality_checks $(TARGET_DIR)/rabin_c $(TARGET_DIR)/blum_goldwasser
#OBJ = $(TARGET_DIR)/encrypt.o $(TARGET_DIR)/decrypt.o $(TARGET_DIR)/keypairgenerator.o $(TARGET_DIR)/bruteforce.o
LIBOBJ	= $(TARGET_DIR)/modular_ops.o $(TARGET_DIR)/primes.o $(TARGET_DIR)/ciphers/elgamal.o $(TARGET_DIR)/ciphers/rsa.o
#SOURCE	= $(SOURCE_DIR)/encrypt.c $(SOURCE_DIR)/decrypt.c $(SOURCE_DIR)/encoding.c $(LIBSOURCE)
#LIBSOURCE = $(LIB_DIR)/modular_ops.c $(LIB_DIR)/primes.c

CC	 	 = gcc
CCFLAGS	 = -Wall -std=c99 $(OPTS)
LFLAGS	 = -lm
INCLUDES = -I$(INCLUDE_DIR)

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    OPTS = -g
else
    OPTS = -O2
endif

.PHONY: clean ch4_testdata dirs

default: dirs build
	@printf '\033[32;1m[OK] Build successful!\033[0m Executables are in $(TARGET_DIR)\n'
	make clean

build: ch1 ch2 ch2_1 ch3 ch3_1 ch4 ch5 ch6 ch7


# Ciphers libraries
$(TARGET_DIR)/rsa.o: $(SOURCE_DIR)/lib/ciphers/rsa.c
	$(CC) -c $(CCFLAGS) $(INCLUDES) $< -o $(TARGET_DIR)/rsa.o $(LFLAGS)

$(TARGET_DIR)/elgamal.o: $(SOURCE_DIR)/lib/ciphers/elgamal.c
	$(CC) -c $(CCFLAGS) $(INCLUDES) $< -o $(TARGET_DIR)/elgamal.o $(LFLAGS)

$(TARGET_DIR)/rabin.o: $(SOURCE_DIR)/lib/ciphers/rabin.c
	$(CC) -c $(CCFLAGS) $(INCLUDES) $< -o $(TARGET_DIR)/rabin.o $(LFLAGS)

$(TARGET_DIR)/blum-goldwasser.o: $(SOURCE_DIR)/lib/ciphers/blum-goldwasser.c
	$(CC) -c $(CCFLAGS) $(INCLUDES) $< -o $(TARGET_DIR)/blum-goldwasser.o $(LFLAGS)

$(TARGET_DIR)/%.o: $(SOURCE_DIR)/lib/%.c
	$(CC) -c $(CCFLAGS) $(INCLUDES) $< -o $(TARGET_DIR)/$*.o $(LFLAGS)


#
#	CHAPTER 1: Prerequisites (Modular Arithmetic, Fast GCD computation, Powermod)
#


ch1: $(TARGET_DIR)/modular_arithmetic/modinv $(TARGET_DIR)/modular_arithmetic/modpow
ch1_deps := $(TARGET_DIR)/modular_ops.o

$(TARGET_DIR)/modular_arithmetic/%: $(SOURCE_DIR)/1_modular_arithmetic/%.c $(ch1_deps)	# this is a pattern rule, it matches any file in the target directory for chapter 1
	$(CC) $(CCFLAGS) $(INCLUDES) $(ch1_deps) $< -o $(TARGET_DIR)/modular_arithmetic/$* $(LFLAGS)



#
#	CHAPTER 2: RSA Cryptosystem and Attacks
#


ch2 ch2_1: $(ch2_deps)
ch2_deps := $(TARGET_DIR)/modular_ops.o $(TARGET_DIR)/rsa.o

ch2: $(TARGET_DIR)/rsa_c/rsa_message $(TARGET_DIR)/rsa_c/rsa_encrypt $(TARGET_DIR)/rsa_c/rsa_decrypt $(TARGET_DIR)/rsa_c/rsa_keypair $(TARGET_DIR)/rsa_c/rsa_bruteforce
ch2_1: $(TARGET_DIR)/rsa_c/attacks/chosen_ciphertext_attack_example $(TARGET_DIR)/rsa_c/attacks/cycle_attack_example

$(TARGET_DIR)/rsa_c/%: $(SOURCE_DIR)/2_rsa/%.c $(ch2_deps)	# this rule matches chapter 2 rsa examples
	$(CC) $(CCFLAGS) $(INCLUDES) $(ch2_deps) $< -o $(TARGET_DIR)/rsa_c/$* $(LFLAGS)

#$(TARGET_DIR)/rsa_c/attacks/%: $(SOURCE_DIR)/2_rsa/attacks/%.c $(ch2_deps)	# this rule matches chapter 2-1 rsa attacks
#	$(CC) $(CCFLAGS) $(INCLUDES) $(ch2_deps) $< -o $(TARGET_DIR)/rsa_c/attacks/$* $(LFLAGS)


#
#	CHAPTER 3: ElGamal cryptosystem, signature scheme and Attacks
#

ch3 ch3_1: $(ch3_deps)
ch3_deps := $(TARGET_DIR)/modular_ops.o $(TARGET_DIR)/elgamal.o


ch3: $(TARGET_DIR)/elgamal_c/elgamal $(TARGET_DIR)/elgamal_c/elgamal_encrypt $(TARGET_DIR)/elgamal_c/elgamal_decrypt $(TARGET_DIR)/elgamal_c/elgamal_sign
ch3_1: $(TARGET_DIR)/elgamal_c/nonce_reuse_vulnerability_example

$(TARGET_DIR)/elgamal_c/%: $(SOURCE_DIR)/3_elgamal/%.c $(ch3_deps)	# this is a pattern rule, it matches any file in the target directory for chapter 3
	$(CC) $(CCFLAGS) $(INCLUDES) $(ch3_deps) $< -o $(TARGET_DIR)/elgamal_c/$* $(LFLAGS)
	



#
#	CHAPTER 4: Text message encoding and encryption
#

ch4 : $(ch4_deps)
ch4_deps := $(TARGET_DIR)/encoding.o $(TARGET_DIR)/modular_ops.o $(TARGET_DIR)/rsa.o $(TARGET_DIR)/primes.o

ch4: $(TARGET_DIR)/text_message_encoding/encrypt $(TARGET_DIR)/text_message_encoding/decrypt $(TARGET_DIR)/text_message_encoding/keypair_generator

$(TARGET_DIR)/text_message_encoding/%: $(SOURCE_DIR)/4_text_message_encoding/%.c $(ch4_deps)
	$(CC) $(CCFLAGS) $(INCLUDES) $(ch4_deps) $< -o $(TARGET_DIR)/text_message_encoding/$* $(LFLAGS)




#
#	CHAPTER 5: Primality checks
#

ch5 : $(ch5_deps)
ch5_deps := $(TARGET_DIR)/modular_ops.o $(TARGET_DIR)/primes.o

ch5: $(TARGET_DIR)/primality_checks/sieve $(TARGET_DIR)/primality_checks/miller_rabin

$(TARGET_DIR)/primality_checks/%: $(SOURCE_DIR)/5_primality_checks/%.c $(ch5_deps)
	$(CC) $(CCFLAGS) $(INCLUDES) $(ch5_deps) $< -o $(TARGET_DIR)/primality_checks/$* $(LFLAGS)




#
#	CHAPTER 6: Rabin cryptosystem
#

ch6 : $(ch6_deps)
ch6_deps := $(TARGET_DIR)/modular_ops.o $(TARGET_DIR)/encoding.o $(TARGET_DIR)/primes.o $(TARGET_DIR)/rabin.o

ch6: $(TARGET_DIR)/rabin_c/rabin $(TARGET_DIR)/rabin_c/rabin_encrypt $(TARGET_DIR)/rabin_c/rabin_decrypt $(TARGET_DIR)/rabin_c/rabin_keypair

$(TARGET_DIR)/rabin_c/%: $(SOURCE_DIR)/6_rabin/%.c $(ch6_deps)
	$(CC) $(CCFLAGS) $(INCLUDES) $(ch6_deps) $< -o $(TARGET_DIR)/rabin_c/$* $(LFLAGS)




#
#	CHAPTER 7: Blum-Goldwasser, Secure random number generation
#

ch7 : $(ch7_deps)
ch7_deps := $(TARGET_DIR)/modular_ops.o $(TARGET_DIR)/primes.o $(TARGET_DIR)/bbs.o $(TARGET_DIR)/blum-goldwasser.o

ch7: $(TARGET_DIR)/blum_goldwasser/bbs_gen $(TARGET_DIR)/blum_goldwasser/blum_prime $(TARGET_DIR)/blum_goldwasser/blum_goldwasser $(TARGET_DIR)/blum_goldwasser/bg_encrypt $(TARGET_DIR)/blum_goldwasser/bg_decrypt $(TARGET_DIR)/blum_goldwasser/bg_keypair

$(TARGET_DIR)/blum_goldwasser/%: $(SOURCE_DIR)/7_blum_goldwasser/%.c $(ch7_deps)
	$(CC) $(CCFLAGS) $(INCLUDES) $(ch7_deps) $< -o $(TARGET_DIR)/blum_goldwasser/$* $(LFLAGS)





dirs: 
	@$(MKDIR_P) $(TARGET_DIRECTORIES)

ch4_testdata:
	cp -r $(SOURCE_DIR)/4_text_message_encoding/testdata/* $(TARGET_DIR)/text_message_encoding/

clean:
	rm -r $(TARGET_DIR)/*.o

success:
	@echo "\033[32;1m[OK] Build successful!\033[0m Executables are in ./build/"