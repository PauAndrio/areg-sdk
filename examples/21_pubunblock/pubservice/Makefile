21_pubservice_PROJECT_NAME := 21_pubservice

21_pubservice_TARGET_BIN := $(21_pubservice_PROJECT_NAME)$(AREG_BIN_EXT)
21_pubservice_OUTPUT_OBJ := $(21_pubunblock_OUTPUT_OBJ)/pubservice

21_pubservice_SRC  :=
21_pubservice_OBJS :=

include $(21_pubunblock_BASE)/pubservice/src/Makefile

# define one target for each source file
$(foreach cpp, $(21_pubservice_SRC), $(eval $(call obj, $(cpp), $(21_pubservice_OUTPUT_OBJ), 21_pubservice_OBJS, $(21_pubunblock_CXXFLAGS))))

DEPS = $(21_pubservice_OBJS:%.o=%.d)
-include $(DEPS)

$(AREG_OUTPUT_BIN)/$(21_pubservice_TARGET_BIN): $(areg_TARGET_PATH) $(AREG_OUTPUT_LIB)/$(21_generated_TARGET_BIN) $(21_pubservice_OBJS)
	@echo "Linking $@ ..."
	@mkdir -p $(dir $@)
	$(AREG_TOOLCHAIN) $(CXXFLAGS) $(21_pubunblock_CXXFLAGS) $(21_pubservice_OBJS) $(21_pubunblock_LDFLAGS) -o $@
