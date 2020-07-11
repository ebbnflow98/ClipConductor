#pragma once
#include "ofConstants.h"

///// \class ofFileDialogResult
///// \brief file dialog box selection or save result
//class ofFileDialogResult{
//	public:
//		ofFileDialogResult();
//
//		/// \return the name of the selected file or directory, if set
//		/// currently returns only 1 file, this may change in the future
//		std::string getName();
//
//		/// \return the full path of the selected file or directory, if set
//		std::string getPath();
//
//		std::string filePath; ///< full path to selected file or directory
//		std::string fileName; ///< selected file or directory name
//		bool bSuccess; ///< true if the dialog action was successful, aka file select not cancel
//};
//
///// \brief show an error message in an alert dialog box
//void ofSystemAlertDialog(std::string errorMessage);
//
///// \brief show a file load dialog box
///// \param windowTitle optional window title string, ie. "Load background image"
///// \param bFolderSelection set to true to allow folder selection
///// \param defaultPath optional default directory path to start the dialog in, ie. ofFilePath::getUserHomeDir()
///// \return dialog result with selection (if any)

bool ofSystemYesNoDialog(std::string message); //user added
