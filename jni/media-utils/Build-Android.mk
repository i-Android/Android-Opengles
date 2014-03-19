

define import_all_cpp_file_under
$(patsubst ./%, ./%, \
	$(shell cd $(LOCAL_PATH); \
		find $(1) -name "*.cpp" -and -not -name ".*" -and -not -name "tesp.cpp" ) \
)
endef

#func:import all subdir file.
define import_subdir_cpp_file_under
$(call import_all_cpp_file_under, .)
endef