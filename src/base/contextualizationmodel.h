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
    const static QString NO_IMAGE_URL;  ///< Local URL of no available image representation.
    const static QString NO_IMAGE_PATH; ///< Local path of no available image representation.

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
        QString image = QString(),
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
     * @brief Removes strings with the identifier received by parameter.
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
    void clearStringsList();

    /**
     * @brief Returns the firmware strings list.
     * @return QList<FirmwareString *> &
     */
    QList<FirmwareString *> & getStringsList();

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
     * The model is empty when there aren't any image in the model or the firmware strings list is empty.
     * Return true if model is empty.
     * @return
     */
    bool isEmpty();

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

private:
    QString image;                          ///< Image associated with the model.
    QList<FirmwareString *> stringsList;    ///< List with the strings associated with the model.

signals:

    /**
     * @brief The signal is emited when the image is changed.
     */
    void imageChanged();

    /**
     * @brief The signal is emited when a string is added or removed from the list.
     */
    void stringsListChanged();

    /**
     * @brief The signal is emited when the image is changed and a string is added or removed from the list.
     */
    void modelChanged();
};

#endif // CONTEXTUALIZATIONMODEL_H
