import pandas as pd

from sklearn.pipeline import Pipeline
from sklearn.model_selection import GridSearchCV

from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import GradientBoostingClassifier

from sklearn.feature_selection import VarianceThreshold
from sklearn.feature_selection import SelectKBest


train_data = pd.read_csv('data/train.csv', index_col=0)
train_X = pd.get_dummies(train_data.drop('class', axis=1))
train_y = train_data['class']

test_data_X = pd.read_csv('data/test.csv', index_col=0)
test_X = pd.get_dummies(test_data_X)


# K best:             k
# Variance threshold: threshold

# K neighbors:        n_neighbors
# Random forest:      n_estimators
# Gradient boosting:  n_estimators


def get_model(feature_selection, classification, dictionary):
    pipeline = Pipeline([
        ('feature_selection', feature_selection),
        ('classification', classification)
    ])
    cross_validation = GridSearchCV(pipeline, dictionary, 'accuracy', cv=5)
    cross_validation.fit(train_X, train_y)
    print('score: ', cross_validation.best_score_)
    print('parameters: ', cross_validation.best_params_)
    print()
    return cross_validation


def k_neighbors(with_threshold):
    if with_threshold:
        dictionary = {
            'feature_selection__threshold': [0.75 * 0.25, 0.775 * 0.225, 0.8 * 0.2, 0.825 * 0.175, 0.85 * 0.15],
            'classification__n_neighbors': [3, 5, 7, 9, 11],
        }
        print('-- k neighbors with variance threshold --')
        model = get_model(VarianceThreshold(), KNeighborsClassifier(), dictionary)
    else:
        dictionary = {
            'feature_selection__k': [30, 35, 40, 45, 50],
            'classification__n_neighbors': [17, 19, 21, 23, 25],
        }
        print('-- k neighbors with k best --')
        model = get_model(SelectKBest(), KNeighborsClassifier(), dictionary)
    return model


def random_forest(with_threshold):
    if with_threshold:
        dictionary = {
            'feature_selection__threshold': [0.75 * 0.25, 0.775 * 0.225, 0.8 * 0.2, 0.825 * 0.175, 0.85 * 0.15],
            'classification__n_estimators': [60, 65, 70, 75, 80],
        }
        print('-- random forest with variance threshold --')
        model = get_model(VarianceThreshold(), RandomForestClassifier(random_state=3), dictionary)
    else:
        dictionary = {
            'feature_selection__k': [30, 35, 40, 45, 50],
            'classification__n_estimators': [60, 65, 70, 75, 80],
        }
        print('-- random forest with k best --')
        model = get_model(SelectKBest(), RandomForestClassifier(random_state=3), dictionary)
    return model


def gradient_boosting(with_threshold):
    if with_threshold:
        dictionary = {
            'feature_selection__threshold': [0.75 * 0.25, 0.775 * 0.225, 0.8 * 0.2, 0.825 * 0.175, 0.85 * 0.15],
            'classification__n_estimators': [100, 110, 120, 130, 140, 150, 160, 170, 180, 190],
        }
        print('-- gradient boosting with variance threshold --')
        model = get_model(VarianceThreshold(), GradientBoostingClassifier(random_state=3), dictionary)
    else:
        dictionary = {
            'feature_selection__k': [42, 43, 44, 45, 46, 47, 48, 49, 50, 51],
            'classification__n_estimators': [180, 190, 200, 210, 220, 230, 240, 250, 260, 270]
        }
        print('-- gradient boosting with k best --')
        model = get_model(SelectKBest(), GradientBoostingClassifier(random_state=3), dictionary)
        score:  0.75708257986739
        parameters:  {'classification__n_estimators': 220, 'feature_selection__k': 45}
    return model


def write_prediction(model, file_name):
    prediction = model.predict(test_X)
    print(prediction)
    result = pd.DataFrame(prediction, index=range(1, 6634, 2), columns=['class'])
    result.to_csv(file_name, header=True, index_label='id')


model_kn_th = k_neighbors(with_threshold=True)
write_prediction(model_kn_th, 'prediction/kn_th')

model_kn_kb = k_neighbors(with_threshold=False)
write_prediction(model_kn_kb, 'prediction/kn_kb')

model_rf_th = random_forest(with_threshold=True)
write_prediction(model_rf_th, 'prediction/rf_th')

model_rf_kb = random_forest(with_threshold=False)
write_prediction(model_rf_kb, 'prediction/rf_kb')

model_gb_th = gradient_boosting(with_threshold=True)
write_prediction(model_gb_th, 'prediction/gb_th')

model_gb_kb = gradient_boosting(with_threshold=False)
write_prediction(model_gb_kb, 'prediction/gb_kb')
