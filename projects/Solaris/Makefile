default: release

allclean: debugclean releaseclean

all:	debug release

debug:	ekd cored accrd

release:	ek core accr

debugclean: ekdclean coredclean accrdclean

releaseclean: ekclean coreclean accrclean

ekdclean: cleanekd ekd

ekclean: cleanek ek

coredclean: cleancored cored

coreclean: cleancore core

accrdclean: cleanaccrd accrd

accrclean: cleanaccr accr

ekd:	
	@echo Building Ek debug...
	$(MAKE) -f make_ek CFG=debug
	@echo

ek:	
	@echo Building Ek release...
	$(MAKE) -f make_ek
	@echo

cored:	
	@echo Building PmtCore debug...
	$(MAKE) -f make_core CFG=debug
	@echo

core:
	@echo Building PmtCore release...
	$(MAKE) -f make_core
	@echo

accrd:
	@echo Building PmtAccessor debug...
	$(MAKE) -f make_accr CFG=debug
	@echo 

accr:
	@echo Building PmtAccessor release...
	$(MAKE) -f make_accr
	@echo

cleanekd:
	@echo Cleanup Ek debug...
	$(MAKE) -f make_ek CFG=debug clean
	@echo

cleanek:
	@echo Cleanup Ek release...
	$(MAKE) -f make_ek clean
	@echo

cleancored:
	@echo Cleanup PmtCore debug...
	$(MAKE) -f make_core CFG=debug clean
	@echo

cleancore:
	@echo Cleanup PmtCore release...
	$(MAKE) -f make_core clean
	@echo

cleanaccrd:
	@echo Cleanup PmtAccessor debug...
	$(MAKE) -f make_accr CFG=debug clean
	@echo

cleanaccr:
	@echo Cleanup PmtAccessor release...
	$(MAKE) -f make_accr clean
	@echo


