
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
