#ifndef DATAFLOW_H
#define DATAFLOW_H

//enum data_type_t
//{
//    SignIn,
//    SignUp,
//    getFiles,
//    newFile,
//    deleteFile,
//    DownloadFile,
//    Unknown = -1
//};

//struct data_t
//{
//public:
//        QString login;
//        QString password;
//        QString type;

//        QString message;
//        QVector<QVector<QString> > files;
//        QByteArray file;

//    friend QDataStream &operator << (QDataStream &stream, data_t &object)
//    {
//        stream << object.type;
//        stream << object.login;
//        stream << object.password;
//        stream << object.message;
//        stream << object.files;
//        stream << object.file;

//        return stream;
//    }

//    friend QDataStream &operator >> (QDataStream &stream, data_t &object)
//    {
//        stream >> object.type;
//        stream >> object.login;
//        stream >> object.password;
//        stream >> object.message;
//        stream >> object.files;
//        stream >> object.file;

//        return stream;
//    }
//};


#endif // DATAFLOW_H
