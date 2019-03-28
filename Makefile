FILE_NAME = pub

CUR_SRC_XPP = $(wildcard *.cpp)
CUR_SRC_C = $(wildcard *.c)

CUR_OBJ_XPP = $(patsubst %.cpp, %.o, $(CUR_SRC_XPP))
CUR_OBJ_C =  $(patsubst %.c, %.o, $(CUR_SRC_C))

CUR_DEP_XPP = $(patsubst %.cpp, %.d, $(CUR_SRC_XPP))
CUR_DEP_C = $(patsubst %.c, %.d, $(CUR_SRC_C))

all: genDir $(CUR_OBJ_XPP) $(CUR_OBJ_C) $(CUR_DEP_XPP) $(CUR_DEP_C)
$(CUR_OBJ_XPP):%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@	
	@cp -f $@  $(OBJ_DIR)/$(FILE_NAME)
	
$(CUR_OBJ_C):%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@	
	@cp -f $@  $(OBJ_DIR)/$(FILE_NAME)
	
genDir:
	@test -d $(OBJ_DIR)/$(FILE_NAME) || mkdir -p $(OBJ_DIR)/$(FILE_NAME) 
	@for file in $(CUR_OBJ_XPP);\
	do\
		test -e $(OBJ_DIR)/$(FILE_NAME)/$$file || rm -f $$file;\
	done
	@for file in $(CUR_OBJ_C);\
	do\
		test -e $(OBJ_DIR)/$(FILE_NAME)/$$file || rm -f $$file;\
	done
	
$(CUR_DEP_XPP):%.d:%.cpp
	@rm -f $@;\
	$(CXX) -MM $< >$@.$$$$;\
	sed 's,/($*/)/.o[ :]*,/1.o$@:,g' <$@.$$$$> $@;\
	rm -f $@.$$$$
	
$(CUR_DEP_C):%.d:%.c
	@rm -f $@;/
	$(CC) -MM $< >$@.$$$$;\
	sed 's,/($*/)/.o[ :]*,/1.o$@:,g' <$@.$$$$> $@;\
	rm -f $@.$$$$
#include $(CUR_DEP_XPP)		
#include $(CUR_DEP_C)	
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.d