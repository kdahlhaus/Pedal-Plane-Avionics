// Copyright 2018 by Kevin Dahlhausen

#ifndef _meanfilter_h
#define _meanfilter_h 

template <class T>
class MeanFilter
{

    public:

        MeanFilter(short theSize);
         ~MeanFilter(void);

        void add(T value);

        T mean(void);
        // return mean value in the filter

        void setAllTo(const T& newValue);
        // set all values in the filter to 'newValue'

        T newest(void);


    protected:

        short size;
        short head;
        short meanIndex;
        T *data;
        short *indexSortedByValue;
};

template <class T> T MeanFilter<T>::newest(void)
{
    return data[head];
}

template <class T> T MeanFilter<T>::mean(void)
{
    return data[indexSortedByValue[meanIndex]];
}

template <class T> void MeanFilter<T>::add(T newItem)
{
    head = (++head % size );
    data[head] = newItem;

    //now sort
    short *lowerBound=indexSortedByValue;
    short *upperBound=indexSortedByValue+(size-1);
    short value;
    short *nextElementIndex;
    short *shiftIndex;
    short *shiftIndexMin=lowerBound-1;
    
    for (nextElementIndex = lowerBound+1; nextElementIndex <= upperBound; 
            nextElementIndex++)
    {
        value= *nextElementIndex;
        for (shiftIndex=nextElementIndex-1; shiftIndex != shiftIndexMin && 
                (data[*shiftIndex]>data[value]); shiftIndex--)
        {
            *(shiftIndex+1)=*shiftIndex;
        }
        *(shiftIndex+1)=value;
    }
}

template <class T> MeanFilter<T>::~MeanFilter(void)
{
    delete [] data;
    delete [] indexSortedByValue;
}

template <class T> MeanFilter<T>::MeanFilter(short theSize) :
    size(theSize),
    head(0)
{
    //data = new [] T[size];
    data = new  T[size];
    //indexSortedByValue = new [] short[size];
    indexSortedByValue = new short[size];

    for (short i=0; i<size; i++)
    {
        indexSortedByValue[i]=i;
    }

    meanIndex = size/2;

    setAllTo(0);
}


template <class T> void MeanFilter<T>::setAllTo(const T& newValue)
{
    for (short i=0; i<size; i++)
    {
        data[i] = newValue;
    }
}

#endif
