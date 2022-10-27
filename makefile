BSDIRS=makefile plfiles ad_bitsize makefiles

TARGET=/mnt/p/bitsize

RHOST=luxemburg

RTARGET=Projects/bitsize

SOURCE=.

CP=cp -rv

SCP=scp -r


setup_files:
	@for bs_dir in $(BSDIRS);do \
	(echo "Copying $${bs_dir} to ${RTARGET}";\
	$(SCP) ${SOURCE}/$${bs_dir} amag97@$(RHOST):$(RTARGET))\
	done
