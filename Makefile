
.PHONY: all clean

all:
	
	$(MAKE) -C diablo3-decrypter
	$(MAKE) -C kh25-checksum-fixer
	$(MAKE) -C re-remaster-decrypter
	$(MAKE) -C dmc-decrypter
	$(MAKE) -C naughtydog-decrypter
	$(MAKE) -C re-revelations2-decrypter
	$(MAKE) -C ff13-decrypter
	$(MAKE) -C nfs-rivals-decrypter
	$(MAKE) -C silent-hill3-decrypter
	$(MAKE) -C cod-blackops-decrypter
	$(MAKE) -C ffxhd-checksum-fixer
	$(MAKE) -C nfs-undercover-decrypter
	$(MAKE) -C deadspace-checksum-fixer
	$(MAKE) -C gta5-decrypter
	$(MAKE) -C prototype-checksum-fixer
	$(MAKE) -C dbz-checksum-fixer
	$(MAKE) -C sw4-checksum-fixer
	$(MAKE) -C alien-checksum-fixer
	$(MAKE) -C ducktales-checksum-fixer
	$(MAKE) -C lego-checksum-fixer
	$(MAKE) -C digimon-checksum-fixer
	$(MAKE) -C mgs-hd-decrypter
	$(MAKE) -C mgs-pw-decrypter
	$(MAKE) -C cod-checksum-fixer
	$(MAKE) -C tears2-checksum-fixer
	$(MAKE) -C toz-checksum-fixer
	$(MAKE) -C dw8xl-decrypter

clean:
	$(MAKE) clean -C diablo3-decrypter
	$(MAKE) clean -C kh25-checksum-fixer
	$(MAKE) clean -C re-remaster-decrypter
	$(MAKE) clean -C dmc-decrypter
	$(MAKE) clean -C naughtydog-decrypter
	$(MAKE) clean -C re-revelations2-decrypter
	$(MAKE) clean -C ff13-decrypter
	$(MAKE) clean -C nfs-rivals-decrypter
	$(MAKE) clean -C silent-hill3-decrypter
	$(MAKE) clean -C cod-blackops-decrypter
	$(MAKE) clean -C ffxhd-checksum-fixer
	$(MAKE) clean -C nfs-undercover-decrypter
	$(MAKE) clean -C deadspace-checksum-fixer
	$(MAKE) clean -C gta5-decrypter
	$(MAKE) clean -C prototype-checksum-fixer
	$(MAKE) clean -C dbz-checksum-fixer
	$(MAKE) clean -C sw4-checksum-fixer
	$(MAKE) clean -C alien-checksum-fixer
	$(MAKE) clean -C lego-checksum-fixer
	$(MAKE) clean -C digimon-checksum-fixer
	$(MAKE) clean -C ducktales-checksum-fixer
	$(MAKE) clean -C mgs-pw-decrypter
	$(MAKE) clean -C mgs-hd-decrypter
	$(MAKE) clean -C cod-checksum-fixer
	$(MAKE) clean -C tears2-checksum-fixer
	$(MAKE) clean -C toz-checksum-fixer
	$(MAKE) clean -C dw8xl-decrypter
