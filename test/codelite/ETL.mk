##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=ETL
ConfigurationName      :=Debug
WorkspacePath          := "U:\Users\John\Documents\Programming\GitHub\etl\test\codelite"
ProjectPath            := "U:\Users\John\Documents\Programming\GitHub\etl\test\codelite"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=John
Date                   :=11/05/14
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe 
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)ETL_USE_EXCEPTIONS 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="ETL.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe 
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../../unittest-cpp 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe 
CC       := C:/MinGW/bin/gcc.exe 
CXXFLAGS :=  -g -O0 -Wall -std=c++11 -Wno-unused-variable $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe 


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
Objects0=$(IntermediateDirectory)/UnitTest++_AssertException.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_Checks.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_Test.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/UnitTest++_TestList.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_TestResults.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(ObjectSuffix) $(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_array.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_container.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/test_test_crc.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_cyclic_value.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_deque.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_enum_type.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_exception.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_function.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_largest.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_list.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_math.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_numeric.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/test_test_observer.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_queue.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_stack.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_type_traits.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_vector.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_test_visitor.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/etl_crc8_ccitt.cpp$(ObjectSuffix) $(IntermediateDirectory)/etl_crc16.cpp$(ObjectSuffix) $(IntermediateDirectory)/etl_crc16_ccitt.cpp$(ObjectSuffix) $(IntermediateDirectory)/etl_crc32.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/etl_crc64_ecma.cpp$(ObjectSuffix) $(IntermediateDirectory)/Win32_TimeHelpers.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	Debug/ETL.exe
	@echo Done

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/UnitTest++_AssertException.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/AssertException.cpp $(IntermediateDirectory)/UnitTest++_AssertException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/AssertException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_AssertException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_AssertException.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/AssertException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_AssertException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_AssertException.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/AssertException.cpp"

$(IntermediateDirectory)/UnitTest++_AssertException.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/AssertException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_AssertException.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/AssertException.cpp"

$(IntermediateDirectory)/UnitTest++_Checks.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/Checks.cpp $(IntermediateDirectory)/UnitTest++_Checks.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/Checks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_Checks.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_Checks.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/Checks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_Checks.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_Checks.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/Checks.cpp"

$(IntermediateDirectory)/UnitTest++_Checks.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/Checks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_Checks.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/Checks.cpp"

$(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/CompositeTestReporter.cpp $(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/CompositeTestReporter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/CompositeTestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/CompositeTestReporter.cpp"

$(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/CompositeTestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_CompositeTestReporter.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/CompositeTestReporter.cpp"

$(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/CurrentTest.cpp $(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/CurrentTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/CurrentTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/CurrentTest.cpp"

$(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/CurrentTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_CurrentTest.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/CurrentTest.cpp"

$(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/DeferredTestReporter.cpp $(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/DeferredTestReporter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/DeferredTestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/DeferredTestReporter.cpp"

$(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/DeferredTestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_DeferredTestReporter.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/DeferredTestReporter.cpp"

$(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/DeferredTestResult.cpp $(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/DeferredTestResult.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/DeferredTestResult.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/DeferredTestResult.cpp"

$(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/DeferredTestResult.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_DeferredTestResult.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/DeferredTestResult.cpp"

$(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/MemoryOutStream.cpp $(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/MemoryOutStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/MemoryOutStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/MemoryOutStream.cpp"

$(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/MemoryOutStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_MemoryOutStream.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/MemoryOutStream.cpp"

$(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/ReportAssert.cpp $(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/ReportAssert.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/ReportAssert.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/ReportAssert.cpp"

$(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/ReportAssert.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_ReportAssert.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/ReportAssert.cpp"

$(IntermediateDirectory)/UnitTest++_Test.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/Test.cpp $(IntermediateDirectory)/UnitTest++_Test.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/Test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_Test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_Test.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/Test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_Test.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_Test.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/Test.cpp"

$(IntermediateDirectory)/UnitTest++_Test.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/Test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_Test.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/Test.cpp"

$(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/TestDetails.cpp $(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/TestDetails.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/TestDetails.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/TestDetails.cpp"

$(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/TestDetails.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_TestDetails.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/TestDetails.cpp"

$(IntermediateDirectory)/UnitTest++_TestList.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/TestList.cpp $(IntermediateDirectory)/UnitTest++_TestList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/TestList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_TestList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_TestList.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/TestList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_TestList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_TestList.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/TestList.cpp"

$(IntermediateDirectory)/UnitTest++_TestList.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/TestList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_TestList.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/TestList.cpp"

$(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/TestReporter.cpp $(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/TestReporter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/TestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/TestReporter.cpp"

$(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/TestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_TestReporter.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/TestReporter.cpp"

$(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/TestReporterStdout.cpp $(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/TestReporterStdout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/TestReporterStdout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/TestReporterStdout.cpp"

$(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/TestReporterStdout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_TestReporterStdout.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/TestReporterStdout.cpp"

$(IntermediateDirectory)/UnitTest++_TestResults.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/TestResults.cpp $(IntermediateDirectory)/UnitTest++_TestResults.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/TestResults.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_TestResults.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_TestResults.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/TestResults.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_TestResults.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_TestResults.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/TestResults.cpp"

$(IntermediateDirectory)/UnitTest++_TestResults.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/TestResults.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_TestResults.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/TestResults.cpp"

$(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/TestRunner.cpp $(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/TestRunner.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/TestRunner.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/TestRunner.cpp"

$(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/TestRunner.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_TestRunner.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/TestRunner.cpp"

$(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/TimeConstraint.cpp $(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/TimeConstraint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/TimeConstraint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/TimeConstraint.cpp"

$(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/TimeConstraint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_TimeConstraint.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/TimeConstraint.cpp"

$(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/XmlTestReporter.cpp $(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/XmlTestReporter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/XmlTestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/XmlTestReporter.cpp"

$(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/XmlTestReporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UnitTest++_XmlTestReporter.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/XmlTestReporter.cpp"

$(IntermediateDirectory)/test_main.cpp$(ObjectSuffix): ../main.cpp $(IntermediateDirectory)/test_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_main.cpp$(DependSuffix): ../main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_main.cpp$(DependSuffix) -MM "../main.cpp"

$(IntermediateDirectory)/test_main.cpp$(PreprocessSuffix): ../main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_main.cpp$(PreprocessSuffix) "../main.cpp"

$(IntermediateDirectory)/test_test_array.cpp$(ObjectSuffix): ../test_array.cpp $(IntermediateDirectory)/test_test_array.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_array.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_array.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_array.cpp$(DependSuffix): ../test_array.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_array.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_array.cpp$(DependSuffix) -MM "../test_array.cpp"

$(IntermediateDirectory)/test_test_array.cpp$(PreprocessSuffix): ../test_array.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_array.cpp$(PreprocessSuffix) "../test_array.cpp"

$(IntermediateDirectory)/test_test_container.cpp$(ObjectSuffix): ../test_container.cpp $(IntermediateDirectory)/test_test_container.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_container.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_container.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_container.cpp$(DependSuffix): ../test_container.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_container.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_container.cpp$(DependSuffix) -MM "../test_container.cpp"

$(IntermediateDirectory)/test_test_container.cpp$(PreprocessSuffix): ../test_container.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_container.cpp$(PreprocessSuffix) "../test_container.cpp"

$(IntermediateDirectory)/test_test_crc.cpp$(ObjectSuffix): ../test_crc.cpp $(IntermediateDirectory)/test_test_crc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_crc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_crc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_crc.cpp$(DependSuffix): ../test_crc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_crc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_crc.cpp$(DependSuffix) -MM "../test_crc.cpp"

$(IntermediateDirectory)/test_test_crc.cpp$(PreprocessSuffix): ../test_crc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_crc.cpp$(PreprocessSuffix) "../test_crc.cpp"

$(IntermediateDirectory)/test_test_cyclic_value.cpp$(ObjectSuffix): ../test_cyclic_value.cpp $(IntermediateDirectory)/test_test_cyclic_value.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_cyclic_value.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_cyclic_value.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_cyclic_value.cpp$(DependSuffix): ../test_cyclic_value.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_cyclic_value.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_cyclic_value.cpp$(DependSuffix) -MM "../test_cyclic_value.cpp"

$(IntermediateDirectory)/test_test_cyclic_value.cpp$(PreprocessSuffix): ../test_cyclic_value.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_cyclic_value.cpp$(PreprocessSuffix) "../test_cyclic_value.cpp"

$(IntermediateDirectory)/test_test_deque.cpp$(ObjectSuffix): ../test_deque.cpp $(IntermediateDirectory)/test_test_deque.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_deque.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_deque.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_deque.cpp$(DependSuffix): ../test_deque.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_deque.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_deque.cpp$(DependSuffix) -MM "../test_deque.cpp"

$(IntermediateDirectory)/test_test_deque.cpp$(PreprocessSuffix): ../test_deque.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_deque.cpp$(PreprocessSuffix) "../test_deque.cpp"

$(IntermediateDirectory)/test_test_enum_type.cpp$(ObjectSuffix): ../test_enum_type.cpp $(IntermediateDirectory)/test_test_enum_type.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_enum_type.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_enum_type.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_enum_type.cpp$(DependSuffix): ../test_enum_type.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_enum_type.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_enum_type.cpp$(DependSuffix) -MM "../test_enum_type.cpp"

$(IntermediateDirectory)/test_test_enum_type.cpp$(PreprocessSuffix): ../test_enum_type.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_enum_type.cpp$(PreprocessSuffix) "../test_enum_type.cpp"

$(IntermediateDirectory)/test_test_exception.cpp$(ObjectSuffix): ../test_exception.cpp $(IntermediateDirectory)/test_test_exception.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_exception.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_exception.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_exception.cpp$(DependSuffix): ../test_exception.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_exception.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_exception.cpp$(DependSuffix) -MM "../test_exception.cpp"

$(IntermediateDirectory)/test_test_exception.cpp$(PreprocessSuffix): ../test_exception.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_exception.cpp$(PreprocessSuffix) "../test_exception.cpp"

$(IntermediateDirectory)/test_test_function.cpp$(ObjectSuffix): ../test_function.cpp $(IntermediateDirectory)/test_test_function.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_function.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_function.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_function.cpp$(DependSuffix): ../test_function.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_function.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_function.cpp$(DependSuffix) -MM "../test_function.cpp"

$(IntermediateDirectory)/test_test_function.cpp$(PreprocessSuffix): ../test_function.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_function.cpp$(PreprocessSuffix) "../test_function.cpp"

$(IntermediateDirectory)/test_test_largest.cpp$(ObjectSuffix): ../test_largest.cpp $(IntermediateDirectory)/test_test_largest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_largest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_largest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_largest.cpp$(DependSuffix): ../test_largest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_largest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_largest.cpp$(DependSuffix) -MM "../test_largest.cpp"

$(IntermediateDirectory)/test_test_largest.cpp$(PreprocessSuffix): ../test_largest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_largest.cpp$(PreprocessSuffix) "../test_largest.cpp"

$(IntermediateDirectory)/test_test_list.cpp$(ObjectSuffix): ../test_list.cpp $(IntermediateDirectory)/test_test_list.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_list.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_list.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_list.cpp$(DependSuffix): ../test_list.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_list.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_list.cpp$(DependSuffix) -MM "../test_list.cpp"

$(IntermediateDirectory)/test_test_list.cpp$(PreprocessSuffix): ../test_list.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_list.cpp$(PreprocessSuffix) "../test_list.cpp"

$(IntermediateDirectory)/test_test_math.cpp$(ObjectSuffix): ../test_math.cpp $(IntermediateDirectory)/test_test_math.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_math.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_math.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_math.cpp$(DependSuffix): ../test_math.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_math.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_math.cpp$(DependSuffix) -MM "../test_math.cpp"

$(IntermediateDirectory)/test_test_math.cpp$(PreprocessSuffix): ../test_math.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_math.cpp$(PreprocessSuffix) "../test_math.cpp"

$(IntermediateDirectory)/test_test_numeric.cpp$(ObjectSuffix): ../test_numeric.cpp $(IntermediateDirectory)/test_test_numeric.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_numeric.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_numeric.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_numeric.cpp$(DependSuffix): ../test_numeric.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_numeric.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_numeric.cpp$(DependSuffix) -MM "../test_numeric.cpp"

$(IntermediateDirectory)/test_test_numeric.cpp$(PreprocessSuffix): ../test_numeric.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_numeric.cpp$(PreprocessSuffix) "../test_numeric.cpp"

$(IntermediateDirectory)/test_test_observer.cpp$(ObjectSuffix): ../test_observer.cpp $(IntermediateDirectory)/test_test_observer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_observer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_observer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_observer.cpp$(DependSuffix): ../test_observer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_observer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_observer.cpp$(DependSuffix) -MM "../test_observer.cpp"

$(IntermediateDirectory)/test_test_observer.cpp$(PreprocessSuffix): ../test_observer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_observer.cpp$(PreprocessSuffix) "../test_observer.cpp"

$(IntermediateDirectory)/test_test_queue.cpp$(ObjectSuffix): ../test_queue.cpp $(IntermediateDirectory)/test_test_queue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_queue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_queue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_queue.cpp$(DependSuffix): ../test_queue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_queue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_queue.cpp$(DependSuffix) -MM "../test_queue.cpp"

$(IntermediateDirectory)/test_test_queue.cpp$(PreprocessSuffix): ../test_queue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_queue.cpp$(PreprocessSuffix) "../test_queue.cpp"

$(IntermediateDirectory)/test_test_stack.cpp$(ObjectSuffix): ../test_stack.cpp $(IntermediateDirectory)/test_test_stack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_stack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_stack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_stack.cpp$(DependSuffix): ../test_stack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_stack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_stack.cpp$(DependSuffix) -MM "../test_stack.cpp"

$(IntermediateDirectory)/test_test_stack.cpp$(PreprocessSuffix): ../test_stack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_stack.cpp$(PreprocessSuffix) "../test_stack.cpp"

$(IntermediateDirectory)/test_test_type_traits.cpp$(ObjectSuffix): ../test_type_traits.cpp $(IntermediateDirectory)/test_test_type_traits.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_type_traits.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_type_traits.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_type_traits.cpp$(DependSuffix): ../test_type_traits.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_type_traits.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_type_traits.cpp$(DependSuffix) -MM "../test_type_traits.cpp"

$(IntermediateDirectory)/test_test_type_traits.cpp$(PreprocessSuffix): ../test_type_traits.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_type_traits.cpp$(PreprocessSuffix) "../test_type_traits.cpp"

$(IntermediateDirectory)/test_test_vector.cpp$(ObjectSuffix): ../test_vector.cpp $(IntermediateDirectory)/test_test_vector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_vector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_vector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_vector.cpp$(DependSuffix): ../test_vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_vector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_vector.cpp$(DependSuffix) -MM "../test_vector.cpp"

$(IntermediateDirectory)/test_test_vector.cpp$(PreprocessSuffix): ../test_vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_vector.cpp$(PreprocessSuffix) "../test_vector.cpp"

$(IntermediateDirectory)/test_test_visitor.cpp$(ObjectSuffix): ../test_visitor.cpp $(IntermediateDirectory)/test_test_visitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/test/test_visitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_test_visitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_visitor.cpp$(DependSuffix): ../test_visitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_visitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_visitor.cpp$(DependSuffix) -MM "../test_visitor.cpp"

$(IntermediateDirectory)/test_test_visitor.cpp$(PreprocessSuffix): ../test_visitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_visitor.cpp$(PreprocessSuffix) "../test_visitor.cpp"

$(IntermediateDirectory)/etl_crc8_ccitt.cpp$(ObjectSuffix): ../../crc8_ccitt.cpp $(IntermediateDirectory)/etl_crc8_ccitt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/crc8_ccitt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/etl_crc8_ccitt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/etl_crc8_ccitt.cpp$(DependSuffix): ../../crc8_ccitt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/etl_crc8_ccitt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/etl_crc8_ccitt.cpp$(DependSuffix) -MM "../../crc8_ccitt.cpp"

$(IntermediateDirectory)/etl_crc8_ccitt.cpp$(PreprocessSuffix): ../../crc8_ccitt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/etl_crc8_ccitt.cpp$(PreprocessSuffix) "../../crc8_ccitt.cpp"

$(IntermediateDirectory)/etl_crc16.cpp$(ObjectSuffix): ../../crc16.cpp $(IntermediateDirectory)/etl_crc16.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/crc16.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/etl_crc16.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/etl_crc16.cpp$(DependSuffix): ../../crc16.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/etl_crc16.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/etl_crc16.cpp$(DependSuffix) -MM "../../crc16.cpp"

$(IntermediateDirectory)/etl_crc16.cpp$(PreprocessSuffix): ../../crc16.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/etl_crc16.cpp$(PreprocessSuffix) "../../crc16.cpp"

$(IntermediateDirectory)/etl_crc16_ccitt.cpp$(ObjectSuffix): ../../crc16_ccitt.cpp $(IntermediateDirectory)/etl_crc16_ccitt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/crc16_ccitt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/etl_crc16_ccitt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/etl_crc16_ccitt.cpp$(DependSuffix): ../../crc16_ccitt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/etl_crc16_ccitt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/etl_crc16_ccitt.cpp$(DependSuffix) -MM "../../crc16_ccitt.cpp"

$(IntermediateDirectory)/etl_crc16_ccitt.cpp$(PreprocessSuffix): ../../crc16_ccitt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/etl_crc16_ccitt.cpp$(PreprocessSuffix) "../../crc16_ccitt.cpp"

$(IntermediateDirectory)/etl_crc32.cpp$(ObjectSuffix): ../../crc32.cpp $(IntermediateDirectory)/etl_crc32.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/crc32.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/etl_crc32.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/etl_crc32.cpp$(DependSuffix): ../../crc32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/etl_crc32.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/etl_crc32.cpp$(DependSuffix) -MM "../../crc32.cpp"

$(IntermediateDirectory)/etl_crc32.cpp$(PreprocessSuffix): ../../crc32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/etl_crc32.cpp$(PreprocessSuffix) "../../crc32.cpp"

$(IntermediateDirectory)/etl_crc64_ecma.cpp$(ObjectSuffix): ../../crc64_ecma.cpp $(IntermediateDirectory)/etl_crc64_ecma.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/etl/crc64_ecma.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/etl_crc64_ecma.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/etl_crc64_ecma.cpp$(DependSuffix): ../../crc64_ecma.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/etl_crc64_ecma.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/etl_crc64_ecma.cpp$(DependSuffix) -MM "../../crc64_ecma.cpp"

$(IntermediateDirectory)/etl_crc64_ecma.cpp$(PreprocessSuffix): ../../crc64_ecma.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/etl_crc64_ecma.cpp$(PreprocessSuffix) "../../crc64_ecma.cpp"

$(IntermediateDirectory)/Win32_TimeHelpers.cpp$(ObjectSuffix): ../../../unittest-cpp/UnitTest++/Win32/TimeHelpers.cpp $(IntermediateDirectory)/Win32_TimeHelpers.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "U:/Users/John/Documents/Programming/GitHub/unittest-cpp/UnitTest++/Win32/TimeHelpers.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Win32_TimeHelpers.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Win32_TimeHelpers.cpp$(DependSuffix): ../../../unittest-cpp/UnitTest++/Win32/TimeHelpers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Win32_TimeHelpers.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Win32_TimeHelpers.cpp$(DependSuffix) -MM "../../../unittest-cpp/UnitTest++/Win32/TimeHelpers.cpp"

$(IntermediateDirectory)/Win32_TimeHelpers.cpp$(PreprocessSuffix): ../../../unittest-cpp/UnitTest++/Win32/TimeHelpers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Win32_TimeHelpers.cpp$(PreprocessSuffix) "../../../unittest-cpp/UnitTest++/Win32/TimeHelpers.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) ".build-debug/ETL"


