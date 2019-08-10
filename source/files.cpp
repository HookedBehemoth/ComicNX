#include "files.hpp"

namespace fs{
    u64 getFreeSpace() {
        u64 space = 0;
        fsFsGetFreeSpace(fsdevGetDefaultFileSystem(), "/", &space);
        return space;
    }
    std::vector<std::string> listDirectory(std::string path) {
        std::vector<std::string> itemList = {};
        DIR* dir;
        struct dirent* ent;
        dir = opendir(path.c_str());
        while((ent = readdir(dir))){
            itemList.push_back(ent->d_name);
        }
        closedir(dir);
        return itemList;
    }
    // https://github.com/AtlasNX/Kosmos-Updater/blob/master/source/FileManager.cpp#L34
    bool writeFile(std::string filename, std::string data) {
        deleteFile(filename);

        FILE * file = fopen(filename.c_str(), "wb");
        if (!file) {
            return false;
        }

        size_t result = fwrite(data.c_str(), sizeof(char), data.size(), file);

        fflush(file);
        fclose(file);

        return (result == data.size());
    }
    // https://github.com/AtlasNX/Kosmos-Updater/blob/master/source/FileManager.cpp#L50
    bool deleteFile(std::string filename) {
        if (fileExists(filename)) {
            return remove(filename.c_str()) == 0;
        }

        return false;
    }
    // https://github.com/AtlasNX/Kosmos-Updater/blob/master/source/FileManager.cpp#L58
    bool fileExists(std::string filename) {
        FILE * file = fopen(filename.c_str(), "r");

        if (file) {
            fflush(file);
            fclose(file);
            return true;
        }

        return false;
    }
    // http://stackoverflow.com/a/11366985
    bool mkpath(std::string path) {
        bool bSuccess = false;
        int nRC = ::mkdir( path.c_str(), 0775 );
        if( nRC == -1 )
        {
            switch(errno)
            {
                case ENOENT:
                    //parent didn't exist, try to create it
                    if( mkpath( path.substr(0, path.find_last_of('/')) ) )
                        //Now, try to create again.
                        bSuccess = 0 == ::mkdir( path.c_str(), 0775 );
                    else
                        bSuccess = false;
                    break;
                case EEXIST:
                    //Done!
                    bSuccess = true;
                    break;
                default:
                    bSuccess = false;
                    break;
            }
        }
        else
            bSuccess = true;
        return bSuccess;
    }

    std::string getSuffix(int fType){
        switch(fType) {
            case model::FType::JPG:
                return ".jpg";
            case model::FType::PNG:
                return ".png";
            case model::FType::GIF:
                return ".gif";
            default:
                return ".unsupported";
        }
    }
}