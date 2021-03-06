/**
 * @file contextualizationmodel.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class ContextualizationModel
 *
 * @brief This is the model class of a MVC architecture on Contextualization Tool app.
 */

#ifndef CONTEXTUALIZATIONMODEL_H
#define CONTEXTUALIZATIONMODEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include "firmwarestring.h"
#include "src/tools/log.h"

class ContextualizationModel : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructs a model.
     *
     * Creates a model setting the image as the image received by parameter. Also adds to the model strings received in
     * the list of the second parameter.
     * If no parameters are received, create an empty model.
     * @param image File path of image to associate to the model.
     * @param list List of strings to add on the model.
     */
    ContextualizationModel(
        QString image = "",
        QList<FirmwareString *> list = QList<FirmwareString *>()
    );

    /**
     * @brief Destroys the model.
     */
    ~ContextualizationModel();

    /**
     * @brief Adds new string to the model.
     * @param id String ID.
     * @param value String value.
     * @param description String description.
     * @param maxLength String max length.
     * @param state String state.
     * @param selected Indicates if the string is selected.
     */
    void addString(
        const QString &id,
        const QString &value,
        const QString &description,
        const QString &maxLength,
        const QString &state,
        const bool selected
    );

    /**
     * @brief Adds new string to the model.
     * @param newString
     */
    void addString(FirmwareString * newString);

    /**
     * @brief Adds new string to the model.
     *
     * Adds all strings in the list received by parameter.
     * @param strings List of strings to add.
     */
    void addStrings(QList<FirmwareString *> &strings);

    /**
     * @brief Removes string with the identifier received by parameter.
     *
     * Returns true if the string removed succesfuly. Returns false if there are not any string with this identifer.
     * @param id Identifier of firmware string to remove.
     * @return bool
     */
    bool removeString(QString &id);

    /**
     * @brief Removes string that are in position received by parameter.
     *
     * Returns true if the string removed succesfuly. Returns false if is not a valid position.
     * @param pos Position of the string in list.
     * @return bool
     */
    bool removeString(int pos);

    /**
     * @brief Removes all items from the firmware strings list.
     */
    void removeAllStrings();

    /**
     * @brief Selects string with the identifier received by parameter.
     *
     * Returns true if the string selected succesfuly. Returns false if there are not any string with this identifer.
     * @param id Identifier of firmware string to remove.
     * @return bool
     */
    bool selectString(const QString id);

    /**
     * @brief Unselects string with the identifier received by parameter.
     *
     * Returns true if the string selected succesfuly. Returns false if there are not any string with this identifer.
     * @param id Identifier of firmware string to remove.
     * @return bool
     */
    bool unselectString(const QString id);


    /**
     * @brief Returns the firmware strings list.
     * @return QList<FirmwareString *> &
     */
    QList<QObject *> &getStringsList();

    /**
     * @brief Sets the image of the model.
     * @param path File path of the image.
     */
    void setImage(QString path);

    /**
     * @brief Returns the image associated to the model.
     * @return QString
     */
    QString getImage();

    /**
     * @brief Checks if the model is empty.
     *
     * The model is empty when there aren't any image in the model and the firmware strings list is empty.
     * Return true if model is empty.
     * @return
     */
    bool isEmpty();

    /**
     * @brief Returns true if the model has a valid image associated.
     * @return bool
     */
    bool hasImage();

    /**
     * @brief Returns true if the model has any string.
     * @return bool.
     */
    bool hasStrings();

    /**
     * @brief Empty the model.
     *
     * Unsets the path of image and removes all items from the firmware strings list.
     */
    void clear();

    /**
     * @brief Returns the model converted to a JSON QString.
     * @param format Exit format of JSON.
     * @return QString
     */
    QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact);

    /**
     * @brief Returns the model converted to a #QJsonObject.
     * @return QJsonObject
     */
    QJsonObject toJsonObject();

    /**
     * @brief Returns a #ContextualizationModel initializated with the JSON string json.
     *
     * If the JSON data received is not valid, return an empty model.
     * @param json String in JSON format to be converted.
     * @return ContextualizationModel
     */
    static ContextualizationModel * fromJson(QString &json);

    /**
     * @brief Returns a #ContextualizationModel initializated with the #QByteArray data json.
     *
     * If the JSON data received is not valid, return an empty model.
     * @param json QByteArray in JSON format to be converted.
     * @return ContextualizationModel
     */
    static ContextualizationModel * fromJson(QByteArray &json);

    /**
     * @brief Assigns other to this model and returns a reference to this model.
     * @param other Model to be copied.
     * @return Reference to this model.
     */
    ContextualizationModel & operator=(ContextualizationModel &other);

signals:

    /**
     * @brief This signal is emited when a string is modified on model.
     */
    void stringsListChanged();

    /**
     * @brief This signal is emited when the image of model is changed.
     */
    void imageChanged();

private:
    QString image_;                 ///< Image associated with the model.
    QList<QObject *> stringsList_;  ///< List with the strings associated with the model.
};

#endif // CONTEXTUALIZATIONMODEL_H
